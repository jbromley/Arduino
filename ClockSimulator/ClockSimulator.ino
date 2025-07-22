#include <TimerOne.h>

// ===================== CONFIGURATION =====================

#define MAX_EVENTS 64  // Ring buffer size

struct Event {
  unsigned long time;   // when to trigger (micros)
  uint8_t pin;          // which pin
  bool state;           // HIGH or LOW
};

struct QuadratureConfig {
  uint8_t startPin;       
  uint16_t numCycles;     
  unsigned long period;   // period of 1 cycle (ms)
  int phaseOffset;        
  unsigned long repeatInterval;  // ms
  unsigned long nextStartTime;   // next periodic batch (ms)
  unsigned long nextAvailableTime; // encoder free time (us)
};

// ===================== STATIC STATE =====================

// Quadrature encoders
static QuadratureConfig quadConfigs[3] = {
  {2, 10, 500,  90, 10000, 0, 0},   // Encoder 1
  {4,  5, 1000, -90, 20000, 0, 0},  // Encoder 2
  {6,  8, 250,  90, 15000, 0, 0}    // Encoder 3
};

// Extra 3-pulse output
const uint8_t pulsePin = 8;
const unsigned long pulseRepeatInterval = 12000;  // ms
static unsigned long pulseNextStartTime = 0;

// Button inputs
const uint8_t buttonPins[3] = {9, 10, 11};
static bool lastButtonState[3] = {true, true, true};
static unsigned long lastDebounceTime[3] = {0,0,0};
const unsigned long debounceDelay = 50; // ms debounce

// Ring buffer
static Event eventRing[MAX_EVENTS];
static volatile uint8_t ringHead = 0;
static volatile uint8_t ringTail = 0;

// ===================== RING BUFFER HELPERS =====================

inline bool ringIsEmpty() { return ringHead == ringTail; }
inline bool ringIsFull()  { return ((ringTail + 1) % MAX_EVENTS) == ringHead; }

bool pushEvent(unsigned long time, uint8_t pin, bool state) {
  if (ringIsFull()) return false;
  eventRing[ringTail].time  = time;
  eventRing[ringTail].pin   = pin;
  eventRing[ringTail].state = state;
  ringTail = (ringTail + 1) % MAX_EVENTS;
  return true;
}

bool peekEvent(Event &ev) {
  if (ringIsEmpty()) return false;
  ev = eventRing[ringHead];
  return true;
}

bool popEvent() {
  if (ringIsEmpty()) return false;
  ringHead = (ringHead + 1) % MAX_EVENTS;
  return true;
}

// ===================== QUADRATURE SCHEDULING =====================

// Convert ms → us
inline unsigned long ms_to_us(unsigned long ms) { return ms * 1000UL; }

void scheduleOneCycle(QuadratureConfig &cfg, unsigned long startUs) {
  unsigned long quarterPeriodUs = ms_to_us(cfg.period) / 4;

  // Channel A transitions
  pushEvent(startUs, cfg.startPin, HIGH);
  pushEvent(startUs + 2 * quarterPeriodUs, cfg.startPin, LOW);

  // Channel B transitions with phase offset
  long phaseShiftUs = (long)(quarterPeriodUs * cfg.phaseOffset / 90);
  unsigned long B_start = startUs + quarterPeriodUs + phaseShiftUs;
  pushEvent(B_start, cfg.startPin + 1, HIGH);
  pushEvent(B_start + 2 * quarterPeriodUs, cfg.startPin + 1, LOW);

  cfg.nextAvailableTime = startUs + ms_to_us(cfg.period);
}

void scheduleQuadratureSequence(QuadratureConfig &cfg, unsigned long nowMs) {
  unsigned long nowUs = micros();
  unsigned long startUs = (nowUs > cfg.nextAvailableTime) ? nowUs : cfg.nextAvailableTime;

  for (uint16_t c = 0; c < cfg.numCycles; c++) {
    unsigned long cycleStartUs = startUs + ms_to_us(cfg.period) * c;
    scheduleOneCycle(cfg, cycleStartUs);
  }

  cfg.nextAvailableTime = startUs + ms_to_us(cfg.numCycles * cfg.period);
  cfg.nextStartTime = nowMs + cfg.repeatInterval;
}

void scheduleQuadratureCycleOnce(uint8_t idx, unsigned long nowMs) {
  QuadratureConfig &cfg = quadConfigs[idx];
  unsigned long nowUs = micros();
  unsigned long startUs = (nowUs > cfg.nextAvailableTime) ? nowUs : cfg.nextAvailableTime;
  scheduleOneCycle(cfg, startUs);
}

// ===================== EXTRA 3-PULSE SEQUENCE =====================

void scheduleThreePulses(unsigned long baseMs) {
  unsigned long baseUs = ms_to_us(baseMs);
  unsigned long pulse1 = 50;  // ms
  unsigned long pulse2 = 100; // ms
  unsigned long pulse3 = 30;  // ms
  unsigned long gap = 200;    // ms

  pushEvent(baseUs, pulsePin, HIGH);
  pushEvent(baseUs + ms_to_us(pulse1), pulsePin, LOW);

  pushEvent(baseUs + ms_to_us(gap), pulsePin, HIGH);
  pushEvent(baseUs + ms_to_us(gap + pulse2), pulsePin, LOW);

  pushEvent(baseUs + ms_to_us(2*gap), pulsePin, HIGH);
  pushEvent(baseUs + ms_to_us(2*gap + pulse3), pulsePin, LOW);

  pulseNextStartTime = baseMs + pulseRepeatInterval;
}

// ===================== TIMER ISR =====================

void timerISR() {
  // Execute current event
  if (!ringIsEmpty()) {
    Event ev = eventRing[ringHead];
    digitalWrite(ev.pin, ev.state);
    popEvent();

    // Schedule next event
    if (!ringIsEmpty()) {
      Event nextEv;
      peekEvent(nextEv);
      unsigned long now = micros();
      unsigned long delayUs = (nextEv.time > now) ? (nextEv.time - now) : 1;
      Timer1.initialize(delayUs);
    }
  }
}

// Program Timer1 for next event
void scheduleNextTimerEvent() {
  if (!ringIsEmpty()) {
    Event nextEv;
    peekEvent(nextEv);
    unsigned long now = micros();
    unsigned long delayUs = (nextEv.time > now) ? (nextEv.time - now) : 1;
    Timer1.initialize(delayUs);
    Timer1.attachInterrupt(timerISR);
  }
}

// ===================== BUTTON HANDLER =====================

void checkButtons() {
  unsigned long nowMs = millis();

  for (uint8_t i = 0; i < 3; i++) {
    bool reading = digitalRead(buttonPins[i]);
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = nowMs;
    }
    if ((nowMs - lastDebounceTime[i]) > debounceDelay) {
      if (lastButtonState[i] == HIGH && reading == LOW) {
        scheduleQuadratureCycleOnce(i, nowMs);
        scheduleNextTimerEvent(); // ensure timer picks up new events
      }
    }
    lastButtonState[i] = reading;
  }
}

// ===================== SETUP =====================

void setup() {
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(quadConfigs[i].startPin, OUTPUT);
    pinMode(quadConfigs[i].startPin + 1, OUTPUT);
  }
  pinMode(pulsePin, OUTPUT);

  for (uint8_t i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  unsigned long nowMs = millis();

  // Schedule initial periodic sequences
  for (uint8_t i = 0; i < 3; i++) {
    scheduleQuadratureSequence(quadConfigs[i], nowMs);
  }

  scheduleThreePulses(nowMs + 2000);

  // Start first event timer
  scheduleNextTimerEvent();
}

// ===================== MAIN LOOP =====================

void loop() {
  unsigned long nowMs = millis();

  // Periodic encoder batches
  for (uint8_t i = 0; i < 3; i++) {
    if ((long)(nowMs - quadConfigs[i].nextStartTime) >= 0) {
      scheduleQuadratureSequence(quadConfigs[i], quadConfigs[i].nextStartTime);
      scheduleNextTimerEvent();
    }
  }

  // Periodic 3-pulse sequence
  if ((long)(nowMs - pulseNextStartTime) >= 0) {
    scheduleThreePulses(pulseNextStartTime);
    scheduleNextTimerEvent();
  }

  // Check buttons
  checkButtons();
}
