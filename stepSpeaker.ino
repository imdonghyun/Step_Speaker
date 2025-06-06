#include "megalovania.h"
#include "unwelcome_school.h"

#define btn 3
#define L_dir 4
#define R_dir 5
#define M1 6
#define ENA 7
#define L_clk 10
#define R_clk 11
#define M0 12

const int (*playlist[][2])[2]={
  {high_note1, low_note1},
  {high_note2, low_note2},
};

float len_list[]={unit_len1, unit_len2};

int pl_idx=0;
int pl_size=1;
int (*high_note)[2] = playlist[pl_idx][0];
int (*low_note)[2] = playlist[pl_idx][1];
float unit_len = len_list[pl_idx];

// typedef struct {
//   unsigned long note_timer;
//   unsigned long beat_timer;
//   unsigned long beat_len;
//   int size;
//   bool pwm;
//   bool dir;
//   int note;
//   int idx;
// } Step;

// Step H = {0, 0, 0, sizeof(high_note)/4, 0, 0, 0};
// Step L = {0, 0, 0, sizeof(low_note)/4, 0, 0, 0};

unsigned long r_timer = 0;
unsigned long l_timer = 0;
unsigned long r_timer2 = 0;
unsigned long l_timer2 = 0;
unsigned long r_len = 0;
unsigned long l_len = 0;
unsigned long now = 0;
bool R = 0;
bool r_dir = 0;

bool L = 0;
bool l_dir = 0;

int r_note=0;
int l_note=0;

int r_idx=0;
int l_idx=0;

bool push=0;



void setup() {
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(R_clk, OUTPUT);
  pinMode(R_dir, OUTPUT);
  pinMode(L_clk, OUTPUT);
  pinMode(L_dir, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btn), next, RISING);

  digitalWrite(R_dir, 1);
  digitalWrite(L_dir, 1);
  digitalWrite(ENA, 1);
  digitalWrite(M0, 1);
  digitalWrite(M1, 1);

  Serial.begin(9600);
}

void initialize() {
  r_timer = 0;
  l_timer = 0;
  r_timer2 = 0;
  l_timer2 = 0;
  r_len=0;
  l_len=0;
  r_note=0;
  l_note=0;
  r_idx=0;
  l_idx=0;
}

void loop() {

  if (push) {
    initialize();
    pl_idx++;
    if (pl_idx>pl_size) pl_idx=0;
    high_note = playlist[pl_idx][0];
    low_note = playlist[pl_idx][1];
    unit_len = len_list[pl_idx];
    Serial.print("song number: ");
    Serial.println(pl_idx);
    delay(1000);
    push=0;
  }

  now = micros();

  if (now - r_timer2 > r_len) {
    r_timer2 = now;
    r_len = pgm_read_word_near(high_note[r_idx]+0);
    r_note= pgm_read_word_near(high_note[r_idx]+1);
    r_len = r_len*unit_len*1000;
    // Serial.println("high");
    // Serial.println(r_idx);
    // Serial.println(r_len);
    r_dir = !r_dir;
    digitalWrite(R_dir, r_dir);
    r_idx++;
    if (r_note==-1) r_idx = 0;
  }

  if (r_note>0 && now - r_timer > r_note) {
    r_timer = now;
    R = !R;
    digitalWrite(R_clk, R);
  }

  if (now - l_timer2 > l_len) {
    l_timer2 = now;
    l_len = pgm_read_word_near(low_note[l_idx]+0);
    l_note= pgm_read_word_near(low_note[l_idx]+1);
    l_len = l_len*unit_len*1000;
    // Serial.println("low");
    // Serial.println(l_idx);
    // Serial.println(l_len);
    l_dir = !l_dir;
    digitalWrite(L_dir, l_dir);
    l_idx++;
    if (l_note == -1) l_idx = 0;
  }
  
  if (l_note>0 && now - l_timer > l_note) {
    l_timer = now;
    L = !L;
    digitalWrite(L_clk, L);
  }
}

void next() {
  push = 1;
}