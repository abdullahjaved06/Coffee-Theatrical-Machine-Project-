#include <Vector.h>

Vector<float> S1_done, S2_done, S3_done;
Vector<int> done_num;

boolean done_ = 0;
float storage_arr1[4], storage_arr2[4], storage_arr3[4];
int storage_arrr[5];
float* reduced_pos[38];

#define HOME_LIMIT 28
#define SB_LIMIT 30

#define jm1_1R 31
#define jm1_1L 29

#define jm1_2R 47
#define jm1_2L 45

#define jm1_3R 44
#define jm1_3L 46

#define jm2_1R 40
#define jm2_1L 42

#define jm2_2R 36
#define jm2_2L 38

#define jm2_3R 32
#define jm2_3L 34

#define dirPin_main 19
#define stepPin_main 17

#define dirPin_SB 15
#define stepPin_SB 13

#define analogLA1 A6
#define LA1_inR 5
#define LA1_inL 3

#define analogLA2 A8
#define LA2_inR 9
#define LA2_inL 7

#define analogLA3 A10
#define LA3_inR 23
#define LA3_inL 21

#define analogLA4 A12
#define LA4_inR 27
#define LA4_inL 25

String msg = "";
char* ptr = NULL;
char* values[5] = { '\0' };

const int STEPS_PER_BOTTLE = 400;
byte PRESENT_BOTTLE = 0;
const int SB_MOTOR_OFFSET = 6;

byte id = 0;
char cup_num = '\0', C_num = '\0', S_num = '\0', SB_num = '\0', J_num = '\0';

float init_LA1 = 0;
float init_LA2 = 0;
float init_LA3 = 0;
float init_LA4 = 0;

float reading = 0, new_reading1 = 0, new_reading2 = 0, new_reading3 = 0, new_reading4 = 0;
float thres1 = 0, thres2 = 0, thres3 = 0, thres4 = 1;
byte stopp = 0;

// Input masks to identify which cup requires what syringe/bottle/pipe's content (randomly initialized for now)
byte S1_in[] = { 0, 0, 1, 0 };
byte S2_in[] = { 0, 1, 0, 0 };
byte S3_in[] = { 1, 0, 0, 0 };

byte SB_in[] = { 0, 0, 0, 0 };
byte SB_type_in1[] = { 0, 0, 0, 0 }; 
byte SB_type_in2[] = { 0, 0, 0, 0 };
byte SB_type_in3[] = { 0, 0, 0, 0 };
byte SB_type_in4[] = { 0, 0, 0, 0};

byte JM1_1_in[] = { 0, 0, 0, 0 };
byte JM1_2_in[] = { 0, 0, 0, 0 };
byte JM1_3_in[] = { 0, 0, 0, 0 };
byte JM2_1_in[] = { 0, 0, 0, 0 };
byte JM2_2_in[] = { 0, 0, 0, 0 };
byte JM2_3_in[] = { 0, 0, 0, 0 };

// Replace the zeros with values when suitable.
//float movement_arr[] = { 0, 0, 0, 5842.42424242,
//                         6443.63636364, 6933.33333333, 8048.48484848, 8800,
//                         9260.60606061, 9842.42424242, 10412.12121212, 11393.93939394,
//                         11927.27272727, 12727.27272727, 13209.6969697, 13648.48484848,
//                         13650.90909091, 13769.6969697, 14259.39393939, 14593.93939394,
//                         15418.18181818, 16160, 16630.3030303, 16812.12121212,
//                         16860.60606061, 17078.78787879, 17587.87878788, 17949.09090909,
//                         18872.72727273, 20048.48484848, 20492.12121212, 21018.18181818,
//                         21389.09090909, 23420.60606061, 23854.54545455, 24242.42424242,
//                         24329.6969697, 32000 };

float movement_arr[] = {  0,             0,             0,          5600,
                         6201.21212121,  6690.90909091,  7806.06060606,  8800,
                         9018.18181818,  9600,         10169.6969697,  11151.51515152,
                        11684.84848485, 12484.84848485, 12967.27272727, 13406.06060606,
                        13408.48484848, 13527.27272727, 14016.96969697, 14351.51515152,
                        15175.75757576, 15917.57575758, 16387.87878788, 16569.6969697,
                        16618.18181818, 16836.36363636, 17345.45454545, 17706.66666667,
                        18630.3030303, 19806.06060606, 20249.6969697,  20775.75757576,
                        21146.66666667, 23178.18181818, 23854.54545455, 24000,
                        24087.27272727, 32000};

float dist = 0;

float* mov_ptr = &movement_arr[0];


float* S1[] = { (mov_ptr + 6), (mov_ptr + 11), (mov_ptr + 19), (mov_ptr + 27) };   // Every point where a cup is present infront of the Syringe 1
float* S2[] = { (mov_ptr + 11), (mov_ptr + 19), (mov_ptr + 27), (mov_ptr + 32) };  // ... Syringe 2
float* S3[] = { (mov_ptr + 19), (mov_ptr + 27), (mov_ptr + 32), (mov_ptr + 36) };  // ... Syringe 3

float* JM1_1[] = { (mov_ptr + 3), (mov_ptr + 8), (mov_ptr + 13), (mov_ptr + 21) };   // ... Juices Manifold 1, pipe 1
float* JM1_2[] = { (mov_ptr + 4), (mov_ptr + 9), (mov_ptr + 14), (mov_ptr + 22) };   // ... Juices Manifold 1, pipe 2
float* JM1_3[] = { (mov_ptr + 5), (mov_ptr + 10), (mov_ptr + 15), (mov_ptr + 23) };  // ... Juices Manifold 1, pipe 3

float* JM2_1[] = { (mov_ptr + 3), (mov_ptr + 8), (mov_ptr + 13), (mov_ptr + 21) };  // ... Juices Manifold 2, pipe 1
float* JM2_2[] = { (mov_ptr + 4), (mov_ptr + 9), (mov_ptr + 14), (mov_ptr + 22) };  // ... Juices Manifold 2, pipe 2
float* JM2_3[] = { (mov_ptr + 5), (mov_ptr + 10), (mov_ptr + 15), (mov_ptr + 23) };  // ... Juices Manifold 2, pipe 3


float* SB[] = { (mov_ptr + 7), (mov_ptr + 12), (mov_ptr + 20), (mov_ptr + 28) };  // ... Syrup Bottles

int idx = 1;
float* temp = nullptr;


float var_mov = 0;
long pos;
bool complete;

void setup()
{
  Serial.begin(9600);
  
  pinMode(stepPin_main, OUTPUT);
  pinMode(dirPin_main, OUTPUT);
  digitalWrite(dirPin_main, LOW);

  pinMode(stepPin_SB, OUTPUT);
  pinMode(dirPin_SB, OUTPUT);
  digitalWrite(dirPin_SB, LOW);

  pinMode(HOME_LIMIT, INPUT_PULLUP);
  pinMode(SB_LIMIT, INPUT_PULLUP);

  pinMode(analogLA1, INPUT);
  pinMode(analogLA2, INPUT);
  pinMode(analogLA3, INPUT);
  pinMode(analogLA4, INPUT);

  pinMode(LA1_inR, OUTPUT);
  pinMode(LA1_inL, OUTPUT);
  digitalWrite(LA1_inR, LOW);
  digitalWrite(LA1_inL, LOW);

  pinMode(LA2_inR, OUTPUT);
  pinMode(LA2_inL, OUTPUT);
  digitalWrite(LA2_inR, LOW);
  digitalWrite(LA2_inL, LOW);

  pinMode(LA3_inR, OUTPUT);
  pinMode(LA3_inL, OUTPUT);
  digitalWrite(LA3_inR, LOW);
  digitalWrite(LA3_inL, LOW);

  pinMode(LA4_inR, OUTPUT);
  pinMode(LA4_inL, OUTPUT);
  digitalWrite(LA4_inR, LOW);
  digitalWrite(LA4_inL, LOW);

  pinMode(jm1_1R, OUTPUT);
  pinMode(jm1_1L, OUTPUT);
  digitalWrite(jm1_1R, LOW);
  digitalWrite(jm1_1L, LOW);

  pinMode(jm1_2R, OUTPUT);
  pinMode(jm1_2L, OUTPUT);
  digitalWrite(jm1_2R, LOW);
  digitalWrite(jm1_2L, LOW);

  pinMode(jm1_3R, OUTPUT);
  pinMode(jm1_3L, OUTPUT);
  digitalWrite(jm1_3R, LOW);
  digitalWrite(jm1_3L, LOW);

  pinMode(jm2_1R, OUTPUT);
  pinMode(jm2_1L, OUTPUT);
  digitalWrite(jm2_1R, LOW);
  digitalWrite(jm2_1L, LOW);

  pinMode(jm2_2R, OUTPUT);
  pinMode(jm2_2L, OUTPUT);
  digitalWrite(jm2_2R, LOW);
  digitalWrite(jm2_2L, LOW);

  pinMode(jm2_3R, OUTPUT);
  pinMode(jm2_3L, OUTPUT);
  digitalWrite(jm2_3R, LOW);
  digitalWrite(jm2_3L, LOW);

  init_LA1 = readPotLA(analogLA1);
  init_LA2 = readPotLA(analogLA2);
  init_LA3 = readPotLA(analogLA3);
  init_LA4 = readPotLA(analogLA4);


//  // Receive and parse string for data
//  while (1) {
//    while (!Serial.available())
//      ;
//    if (Serial.available()) {
//      msg += Serial.read();
//    } else if (msg.length() > 0 && !Serial.available()) {
//      break;
//    }
//  }
//
//  ptr = strtok((char)msg.c_str(), ",");
//
//  cup_num = *ptr;
//  C_num = *ptr;
//  S_num = *ptr;
//  SB_num = *ptr;
//  J_num = *ptr;
//
//  Serial.println(cup_num, C_num, S_num, SB_num, J_num);

  for (byte i = 0; i < sizeof(reduced_pos) / sizeof(reduced_pos[0]); i++) {
    reduced_pos[i] = nullptr;
  }

  // For loops to find the required positions using input masks
  // Positions saved in the "reduced_pos" array.

  reduced_pos[0] = mov_ptr;

  for (byte i = 0; i < sizeof(S1_in) / sizeof(S1_in[0]); i++) {
    if (S1_in[i] == 1)
    {
      reduced_pos[idx] = S1[i];
      Serial.println("Here_S1");
      idx++;
    }
  }

  for (byte i = 0; i < sizeof(S2_in) / sizeof(S2_in[0]); i++) {
    if (S2_in[i] == 1) {
      reduced_pos[idx] = S2[i];
      Serial.println("Here_S2");
      idx++;
    }
  }

  for (byte i = 0; i < sizeof(S3_in) / sizeof(S3_in[0]); i++) {
    if (S3_in[i] == 1)
    {
      reduced_pos[idx] = S3[i];
      Serial.println("Here_S3");
      idx++;
    }
  }

  for (byte i = 0; i < sizeof(JM1_1_in) / sizeof(JM1_1_in[0]); i++) {
    if (JM1_1_in[i] == 1)
    {
      reduced_pos[idx] = JM1_1[i];
      idx++;
    }
  }

  for (byte i = 0; i < sizeof(JM1_1_in) / sizeof(JM1_1_in[0]); i++) {
    if (JM1_2_in[i] == 1)
    {
      reduced_pos[idx] = JM1_2[i];
      idx++;
    }
  }

  for (byte i = 0; i < sizeof(JM1_1_in) / sizeof(JM1_1_in[0]); i++) {
    if (JM1_3_in[i] == 1)
    {
      reduced_pos[idx] = JM1_3[i];
      idx++;
    }
  }

  for (byte i = 0; i < sizeof(JM2_1_in) / sizeof(JM2_1_in[0]); i++) {
    if (JM2_1_in[i] == 1)
    {
      reduced_pos[idx] = JM2_1[i];
      idx++;
    }
  }

  for (byte i = 0; i < sizeof(JM2_1_in) / sizeof(JM2_1_in[0]); i++) {
    if (JM2_2_in[i] == 1)
    {
      reduced_pos[idx] = JM2_2[i];
      idx++;
    }
  }

  for (byte i = 0; i < sizeof(JM2_1_in) / sizeof(JM2_1_in[0]); i++) {
    if (JM2_3_in[i] == 1)
    {
      reduced_pos[idx] = JM2_3[i];
      idx++;
    }
  }

  for (byte i = 0; i < sizeof(SB_in) / sizeof(SB_in[0]); i++) {
    if (SB_in[i] == 1)
    {
      reduced_pos[idx] = SB[i];
      idx++;
    }
  }

  // Sort the reduced positions array (containing addresses) in ascending order
  while (stopp < 45)
  {
    for (int i = 0; i < idx - 1; i++)
    {
      if (reduced_pos[i] > reduced_pos[i + 1])
      {
        temp = reduced_pos[i];
        reduced_pos[i] = reduced_pos[i + 1];
        reduced_pos[i + 1] = temp;
      }
    }
    stopp += 1;
  }

  reduced_pos[idx] = mov_ptr + 37;

  S1_done.setStorage(storage_arr1);
  S1_done.push_back(0);

  S2_done.setStorage(storage_arr2);
  S2_done.push_back(0);

  S3_done.setStorage(storage_arr3);
  S3_done.push_back(0);
  
  done_num.setStorage(storage_arrr);
  done_num.push_back(0);
  
  digitalWrite(dirPin_main, HIGH);
  delay(10);
  while (digitalRead(HOME_LIMIT) == HIGH)
  {
    digitalWrite(stepPin_main, HIGH);
    delayMicroseconds(300);
    digitalWrite(stepPin_main, LOW);
    delayMicroseconds(300);
  }

  delay(500);

  digitalWrite(dirPin_SB, LOW);
  delay(10);
  while (digitalRead(SB_LIMIT) == HIGH)
  {
    digitalWrite(stepPin_SB, HIGH);
    delayMicroseconds(1500);
    digitalWrite(stepPin_SB, LOW);
    delayMicroseconds(1500);
  }
  delay(100);
  
  digitalWrite(dirPin_SB, HIGH);
  delay(10);
  for (int i=0; i < SB_MOTOR_OFFSET; i++)
  {
    digitalWrite(stepPin_SB, HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin_SB, LOW);
    delayMicroseconds(800);
  }

  delay(10);
  digitalWrite(dirPin_SB, LOW);
//  for(byte i = 0; i<4; i++)
//  {
//    Serial.println(*reduced_pos[i]);
//  }
//  Serial.println(" ");
//  for(byte i = 0; i<4; i++)
//  {
//    Serial.println(*S1[i]);
//  }
//  Serial.println(" ");
//  for(byte i = 0; i<4; i++)
//  {
//    Serial.println(*S2[i]);
//  }
//  Serial.println(" ");
//  for(byte i = 0; i<4; i++)
//  {
//    Serial.println(*S3[i]);
//  }
}

void loop()
{
  delay(2000);
  thres1 = init_LA1 + 0.65;
  thres2 = init_LA2 + 0.65;
  thres3 = init_LA3 + 0.65;
  thres4 = init_LA4 + 1;

  // Starts moving to position
  for (byte i = 1; i < sizeof(reduced_pos) / sizeof(reduced_pos[0]); i++)
  {
    digitalWrite(dirPin_main, LOW);
    delay(10);
    dist = (*reduced_pos[i] - *reduced_pos[i - 1]);
    for (int j = 0; j < dist; j++)
    {
      digitalWrite(stepPin_main, HIGH);
      delayMicroseconds(300);
      digitalWrite(stepPin_main, LOW);
      delayMicroseconds(300);
    }
    // Delay after reaching the position
    delay(1000);

    for (byte k = 0; k < (sizeof(S1) / sizeof(S1[0])); k++)
    {
      for(int qq = 0; qq < 4; qq++)
      {
        if (reduced_pos[i] == S1[qq])  // Check if there is any cup infront syringe 1
        {
          delay(800);
          Serial.println("Actuator 1");
          for(int q_check=0; q_check<4; q_check++)
          {
            if(S1_in[q_check] == 1 && q_check == qq && !contain(S1_done, *reduced_pos[i]))
            {
              S1_done.push_back(*reduced_pos[i]);
              delay(10);
              digitalWrite(LA1_inR, LOW);
              digitalWrite(LA1_inL, HIGH);
              delay(100);
              reading = readPotLA(analogLA1);
              while (reading <= thres1)
              {
                reading = readPotLA(analogLA1);
              }
              digitalWrite(LA1_inR, LOW);
              digitalWrite(LA1_inL, LOW);
              new_reading1 = reading;
              delay(1500);
              digitalWrite(LA1_inR, HIGH);
              digitalWrite(LA1_inL, LOW);
              reading = readPotLA(analogLA1);
              while (reading >= new_reading1 - 0.375)
              {
                reading = readPotLA(analogLA1);
                //Serial.println(reading);
              }
              digitalWrite(LA1_inR, LOW);
              digitalWrite(LA1_inL, LOW);
              break;
            }
          }
          break;
        }
      }

      for(int qq = 0; qq < 4; qq++)
      {
        if (reduced_pos[i] == S2[qq])  // Check if there is any cup infront syringe 1
        {
          delay(800);
          Serial.println("Actuator 2");
          for(int q_check=0; q_check<4; q_check++)
          {
            if(S2_in[qq] == 1 && q_check == qq && !contain(S2_done, *reduced_pos[i]))
            {
              S2_done.push_back(*reduced_pos[i]);
              digitalWrite(LA2_inR, LOW);
              digitalWrite(LA2_inL, HIGH);
              delay(100);
              reading = readPotLA(analogLA2);
              while (reading <= thres2)
              {
                reading = readPotLA(analogLA2);
              }
              digitalWrite(LA2_inR, LOW);
              digitalWrite(LA2_inL, LOW);
              new_reading2 = reading;
              delay(1500);
              digitalWrite(LA2_inR, HIGH);
              digitalWrite(LA2_inL, LOW);
              reading = readPotLA(analogLA2);
              while (reading >= new_reading2 - 0.375)
              {
                reading = readPotLA(analogLA2);
                //Serial.println(reading);
              }
              digitalWrite(LA2_inR, LOW);
              digitalWrite(LA2_inL, LOW);
              break;
            }
          }
          break;
        }
      }

      for(int qq = 0; qq < 4; qq++)
      {
        if (reduced_pos[i] == S3[qq])  // Check if there is any cup infront syringe 1
        {
          delay(800);
          Serial.println("Actuator 3");
          for(int q_check=0; q_check<4; q_check++)
          {
            if(S3_in[q_check] == 1 && q_check == qq && !contain(S3_done, *reduced_pos[i]))
            {
              S3_done.push_back(*reduced_pos[i]);
              digitalWrite(LA3_inR, LOW);
              digitalWrite(LA3_inL, HIGH);
              delay(100);
              reading = readPotLA(analogLA3);
              while (reading <= thres3)
              {
                reading = readPotLA(analogLA3);
              }
              digitalWrite(LA3_inR, LOW);
              digitalWrite(LA3_inL, LOW);
              new_reading3 = reading;
              delay(1500);
              digitalWrite(LA3_inR, HIGH);
              digitalWrite(LA3_inL, LOW);
              reading = readPotLA(analogLA3);
              while (reading >= new_reading3 - 0.375)
              {
                reading = readPotLA(analogLA3);
                //Serial.println(reading);
              }
              digitalWrite(LA3_inR, LOW);
              digitalWrite(LA3_inL, LOW);
              break;
            }
          }
          break;
        }
      }

      if(done_ == 1)
      {
        delay(10);
        break;
      }

      if (reduced_pos[i] == SB[k])  // Check if there is any cup infront Syrup Bottle
      {
        delay(800);
        if (k == 0)
        {
          PRESENT_BOTTLE = 0;
          for (byte qq = 0; qq < 4; qq++)
          {
            if (SB_type_in1[qq] == 1)
            {
              digitalWrite(dirPin_SB, HIGH);
              delay(10);
              for (int stp = 0; stp < STEPS_PER_BOTTLE * (qq - PRESENT_BOTTLE); stp++)
              {
                digitalWrite(stepPin_SB, HIGH);
                delayMicroseconds(1500);
                digitalWrite(stepPin_SB, LOW);
                delayMicroseconds(1500);
              }
              delay(500);
              PRESENT_BOTTLE = qq;
              digitalWrite(LA4_inR, LOW);
              digitalWrite(LA4_inL, HIGH);
              delay(100);
              reading = readPotLA(analogLA4);
              while (reading <= thres4)
              {
                reading = readPotLA(analogLA4);
              }
              digitalWrite(LA4_inR, LOW);
              digitalWrite(LA4_inL, LOW);
              new_reading4 = reading;
              delay(500);
              digitalWrite(LA4_inR, HIGH);
              digitalWrite(LA4_inL, LOW);
              reading = readPotLA(analogLA4);
              delay(1800);
              digitalWrite(LA4_inR, LOW);
              digitalWrite(LA4_inL, LOW);
            }
          }
          delay(1000);
          digitalWrite(dirPin_SB, LOW);
          delay(10);
          while (digitalRead(SB_LIMIT) == HIGH)
          {
            digitalWrite(stepPin_SB, HIGH);
            delayMicroseconds(1500);
            digitalWrite(stepPin_SB, LOW);
            delayMicroseconds(1500);
          }
          delay(100);
          digitalWrite(dirPin_SB, HIGH);
        }

        if (k == 1)
        {
          PRESENT_BOTTLE = 0;
          for (byte qq = 0; qq < 4; qq++)
          {
            if (SB_type_in2[qq] == 1)
            {
              digitalWrite(dirPin_SB, HIGH);
              delay(10);
              for (int stp = 0; stp < STEPS_PER_BOTTLE * (qq - PRESENT_BOTTLE); stp++)
              {
                digitalWrite(stepPin_SB, HIGH);
                delayMicroseconds(1500);
                digitalWrite(stepPin_SB, LOW);
                delayMicroseconds(1500);
              }
              delay(500);
              PRESENT_BOTTLE = qq;
              digitalWrite(LA4_inR, LOW);
              digitalWrite(LA4_inL, HIGH);
              delay(100);
              reading = readPotLA(analogLA4);
              while (reading <= thres4)
              {
                reading = readPotLA(analogLA4);
              }
              digitalWrite(LA4_inR, LOW);
              digitalWrite(LA4_inL, LOW);
              new_reading4 = reading;
              delay(500);
              digitalWrite(LA4_inR, HIGH);
              digitalWrite(LA4_inL, LOW);
              reading = readPotLA(analogLA4);
              delay(1800);
              digitalWrite(LA4_inR, LOW);
              digitalWrite(LA4_inL, LOW);
            }
          }
          delay(1000);
          digitalWrite(dirPin_SB, LOW);
          delay(10);
          while (digitalRead(SB_LIMIT) == HIGH)
          {
            digitalWrite(stepPin_SB, HIGH);
            delayMicroseconds(1500);
            digitalWrite(stepPin_SB, LOW);
            delayMicroseconds(1500);
          }
          delay(100);
          digitalWrite(dirPin_SB, HIGH);
        }

        if (k == 2)
        {
          PRESENT_BOTTLE = 0;
          for (byte qq = 0; qq < 4; qq++)
          {
            if (SB_type_in3[qq] == 1)
            {
              digitalWrite(dirPin_SB, HIGH);
              delay(10);
              for (int stp = 0; stp < STEPS_PER_BOTTLE * (qq - PRESENT_BOTTLE); stp++)
              {
                digitalWrite(stepPin_SB, HIGH);
                delayMicroseconds(1500);
                digitalWrite(stepPin_SB, LOW);
                delayMicroseconds(1500);
              }
              delay(500);
              PRESENT_BOTTLE = qq;
              digitalWrite(LA4_inR, LOW);
              digitalWrite(LA4_inL, HIGH);
              delay(100);
              reading = readPotLA(analogLA4);
              while (reading <= thres4)
              {
                reading = readPotLA(analogLA4);
              }
              digitalWrite(LA4_inR, LOW);
              digitalWrite(LA4_inL, LOW);
              new_reading4 = reading;
              delay(500);
              digitalWrite(LA4_inR, HIGH);
              digitalWrite(LA4_inL, LOW);
              reading = readPotLA(analogLA4);
              delay(1800);
              digitalWrite(LA4_inR, LOW);
              digitalWrite(LA4_inL, LOW);
            }
          }
          delay(1000);
          digitalWrite(dirPin_SB, LOW);
          delay(10);
          while (digitalRead(SB_LIMIT) == HIGH)
          {
            digitalWrite(stepPin_SB, HIGH);
            delayMicroseconds(1500);
            digitalWrite(stepPin_SB, LOW);
            delayMicroseconds(1500);
          }
          delay(100);
          digitalWrite(dirPin_SB, HIGH);
        }

        if (k == 3)
        {
          PRESENT_BOTTLE = 0;
          for (byte qq = 0; qq < 4; qq++)
          {
            if (SB_type_in4[qq] == 1)
            {
              digitalWrite(dirPin_SB, HIGH);
              delay(10);
              for (int stp = 0; stp < STEPS_PER_BOTTLE * (qq - PRESENT_BOTTLE); stp++)
              {
                digitalWrite(stepPin_SB, HIGH);
                delayMicroseconds(1500);
                digitalWrite(stepPin_SB, LOW);
                delayMicroseconds(1500);
              }
              delay(500);
              PRESENT_BOTTLE = qq;
              digitalWrite(LA4_inR, LOW);
              digitalWrite(LA4_inL, HIGH);
              delay(100);
              reading = readPotLA(analogLA4);
              while (reading <= thres4)
              {
                reading = readPotLA(analogLA4);
              }
              digitalWrite(LA4_inR, LOW);
              digitalWrite(LA4_inL, LOW);
              new_reading4 = reading;
              delay(500);
              digitalWrite(LA4_inR, HIGH);
              digitalWrite(LA4_inL, LOW);
              reading = readPotLA(analogLA4);
              delay(1800);
              digitalWrite(LA4_inR, LOW);
              digitalWrite(LA4_inL, LOW);
            }
          }
          delay(1000);
          digitalWrite(dirPin_SB, LOW);
          delay(10);
          while (digitalRead(SB_LIMIT) == HIGH)
          {
            digitalWrite(stepPin_SB, HIGH);
            delayMicroseconds(1500);
            digitalWrite(stepPin_SB, LOW);
            delayMicroseconds(1500);
          }
          delay(100);
          digitalWrite(dirPin_SB, HIGH);
        }
      }

      if (reduced_pos[i] == JM1_1[k])  // Check if there is any cup infront JM1, pipe 1
      {
        delay(800);
        runJMpipe(jm1_1R, jm1_1L);
        Serial.println("JM1 P1");
      }

      if (reduced_pos[i] == JM1_2[k])  // Check if there is any cup infront JM1, pipe 2
      {
        delay(800);
        runJMpipe(jm1_2R, jm1_2L);
        Serial.println("JM1 P2");
      }

      if (reduced_pos[i] == JM1_3[k])  // Check if there is any cup infront JM1, pipe 3
      {
        delay(800);
        runJMpipe(jm1_3R, jm1_3L);
        Serial.println("JM1 P3");
      }

      if (reduced_pos[i] == JM2_1[k])  // Check if there is any cup infront JM1, pipe 1
      {
        delay(800);
        runJMpipe(jm1_2R, jm2_1L);
        Serial.println("JM2 P1");
      }

      if (reduced_pos[i] == JM2_2[k])  // Check if there is any cup infront JM1, pipe 2
      {
        delay(800);
        runJMpipe(jm2_2R, jm2_2L);
        Serial.println("JM2 P2");
      }

      if (reduced_pos[i] == JM2_3[k])  // Check if there is any cup infront JM1, pipe 3
      {
        delay(800);
        runJMpipe(jm2_3R, jm2_3L);
        Serial.println("JM2 P3");
      }
    }
  }
  while(1);
}

// Reads the current position of Linear Actuator specified
float readPotLA(const int pin)
{
  return 0.0048876 * analogRead(pin);
}

// Activates the pump that is passed to it
void runJMpipe(const byte in1, const byte in2)
{
  delay(2600);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(2000);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(2600);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(2000);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

bool contain(Vector<float> vec, float num)
{
  for (int v = 0; v < vec.size(); v++)
  {
    if (vec[v] == num)
    {
      return 1;
    }
  }
  return 0;
}
