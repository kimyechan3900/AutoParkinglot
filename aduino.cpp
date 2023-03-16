//��ɺ���=order
//���ۺ���=motion ( ����1=0 , �ݳ�0=1��� ����
//���º���=state[4]  <-ũ�� 4 �迭
//���� 0 = '0', ���� 1 = '1', ���� 2 = '2'��� ����
//�ݳ����=1 , �������='0'��� ����
//��ȣ����=number
//��ǥ����=object
//1.Command  2.State  3.Route  4.Calculator  5.Print  6.Push  7.Floor_Move
#include<Keypad.h>
#include<LiquidCrystal.h>//�з¼���

const byte ROWS = 4;//Ű�е� �κ� ����(Command)
const byte COLS = 3;
char key = 0;
char keys[ROWS][COLS] = {
     {'1','2','3'},
     {'4','5','6'},
     {'7','8','9'},
     {'#','0','*'}
};
byte rowPins[ROWS] = { 6,5};
byte colPins[COLS] = { 4,3,2};
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // Ű�е� �б�

const int sensorpin1 = A0;//�з¼����� ������ �Ƴ��α� ��(state), �ɹ�ȣ���°� �����ؾ���, ��������
const int sensorpin2 = A1;
const int sensorpin3 = A2;
const int sensorpin4 = A3;

//A�� ���� �,B�� �¿� �
const int Dir1Pin_A = 8;
const int Dir2Pin_A = 9;
const int SpeedPin_A = 10;

const int Dir1pin_B = 12;
const int Dir2Pin_B = 13;
const int speedpin_B = 11;


//main
void setup()
{
    Serial.begin(9600);

    pinMode(Dir1Pin_A, OUTPUT);
    pInMode(Dir2Pin_A, OUTPUT);
    pinMode(SpeedPin_A, OUTPUT);
    pinMode(Dir1Pin_B, OUTPUT);
    pInMode(Dir2Pin_B, OUTPUT);
    pinMode(SpeedPin_B, OUTPUT);

}
void loop()
{
    int number = 0, order = 0, state[4] = { 0 }, motion=0, object=0;
    State(state);// �з¼��� ��������
    Serial.println("Ű�е� �Է����ּ���.");
    Command(&number, &order);//Ű�е��Է�
    Serial.println("number����");
    Serail.println(number);
    Serial.println("order����");
    Serial.println(order);
    Route(order, number, state);//����/�ݳ�
  //  Serial.end();//��

}




void Command(int* number, int* order)
{
    while (1)
    {
        key =kpd.getKey();
        if (key != 0)
        {
            switch (key)
            {
            case '6':
                *order = 0;
                *number = 6;
                Serial.println(key);
                break;
            case '1':
                *order = 1;
                *number = 1;
                Serial.println(key);//��¾��ҰŸ� ������
                break;
            case '2':
                *order = 1;
                *number = 2;
                Serial.println(key);
                break;
            case '3':
                *order = 1;
                *number = 3;
                Serial.println(key);
                break;
            case '4':
                *order = 1;
                *number = 4;
                Serial.println(key);
                break;
            default:
                Serial.println("�߸��� key�� �Է��߽��ϴ�.");
            }
        }
    }
}

void State(int *state)
{
    state[0] = analogRead(sensorpin1);
    state[1] = analogRead(sensorpin2);
    state[2] = analogRead(sensorpin3);
    state[3] = analogRead(sensorpin4);

    for (int i = 0;i < 4;i++)
    {
        if (state[i] == 0)
        {
            state[i] = 0;
        }
        else if ((state[i]>=x1)&&(state[i]<=x2) ) //X���� �з¼��������� �����ؾ���.
        {
            state[i] = 1;
        }
        else if (state[i] > X)
        {
            state[i] = 2;
        }
        Serial.print("ù��°");
        Serial.println(state[0]);
        Serial.print("�ι�°");
        Serial.println(state[1]);
        Serial.print("����°");
        Serial.println(state[2]);
        Serial.print("�׹�°");
        Serial.println(state[3]);
        Serial.print("\n");
        delay(3000);

    }
}



void Route(int  order, int number,int* state_p)// 3
{
  
        int object = 0;
        int motion = 0;
        int floor = 0;//������(�����ͷ� �ٲ�)
        int c_floor = 1;//���� ��
        int direction = 0;//���⺯��(�����ͷ� �ٲ�)
        if (order == 0)// @@@@@@����@@@@@@@
        {
       
            for (int i = 0;i < 4;i++)      // 1.������ �ְ� ����0�� ���� ����
            {
                if (state_p[i] == 0)//����0�϶� ����
                {
                    object = i;
                    motion = 0;//���ۺ���=����;
                    Serial.println(object+1);
                    Calculator(object, &floor, &direction);//��,������ �����ؾ��ϹǷ� �����͸��� floor�� direction�� ����
                    Floor_Move(floor,&c_floor);
                    Push(motion, direction);//����
                    state_p[i] = 2;//���������� ���� 2
                    Floor_Move(1, &c_floor);
                    break;// �ݺ��� Ż��
                }
            }
            for (int i = 0;i < 4;i++)// 2. �� �÷��� �켱���������� �ٽ� ������
            {
                if (state_p[i] == 1)
                {
                    object = i;
                    motion = 1;//���ۺ���=�ݳ�;
                    Calculator(object, &floor, &direction);
                    Floor_Move(floor,&c_floor);
                    Push(motion, direction);//�������⶧���� �ݳ������� �˰���
                    state_p[i] = 0;//���÷����� ������ ���� 0
                    Floor_Move(1, &c_floor);
                    break;//�켱���������� �ݺ��� Ż��
                }
            }
        }
        else if (order == 1)//@@@@@@�ݳ�@@@@@@
        {
            if (state_p[0] != 0 || state_p[1] != 0 || state_p[2] != 0 || state_p[3] != 0)// 1. �߾ӿ� ���÷����� �ִ°��
            {
                for (int i = 0;i < 4;i++)      // ���� 0�ΰ� ã�Ƽ� ���÷��� ����
                {
                    if (state_p[i] == 0)//����0�϶� ����
                    {
                        object = i;
                        motion = 0;//���ۺ���=����;
                        Calculator(object, &floor, &direction);//��,������ �����ؾ��ϹǷ� �����͸��� floor�� direction�� ����
                        Floor_Move(floor,&c_floor);
                        Push(motion, direction);//����
                        state_p[i] = 2;//�� �÷��� ���������� ���� 1
                        Floor_Move(1, &c_floor);
                        break;// �ð� �������� �״�����Һ��� ���������ϹǷ� �ݺ��� Ż��
                    }
                }
            }
            object = number - 1; // 2. ��ǰ �ݳ�
            motion = 1;
            Calculator(object, &floor, &direction);
            Push(motion, direction);//�ݳ�
            Floor_Move(floor,&c_floor);
            state_p[object] = 0;//�ݳ������� �װ��� ���´� 0
            Floor_Move(1, &c_floor);
        }
}


//��ǥ���� : target
//�� : floor
//���� : dir  -1�̸� �� 1�̸� ��
void Calculator(int obj, int* flo, int* dir) // 4
{
    int left_right = -1;
    for (int j = 1; j <= 4; j++)
    {
        if (obj == j)
        {
            *flo = (j + 1) / 2;
            *dir = left_right;
        }
        left_right = left_right * (-1);
    }
}

//Ȥ�� ���ܸ��Ϳ� DC���͸� ���� ����� ��������??? ���ܸ��ʹ� ������ �ݳ� ��ȣ�θ� ��밡���� �ݸ� DC���ʹ� ������ �ݳ��ÿ� ���� ���������� �������ؼ� ��ü���� �ڵ���������� 
//���� DC���͸� ���������Ϳ� ����ϰ�, ���ܸ��͸� ���� �¿� PUSH�Լ��� ��� ���� �� �� ������ ������
void Push(int motion, int direction) { //5
    if (motion == 0)// ����
    {
        digitalWrite(Dir1Pin_A, HIGH);//���
        digitalWrite(Dir2Pin_A, LOW);
        analogWrite(SpeedPin_A, 1);//���ڴ� �ٲٸ��(1cm ȸ��)
        
        if (direction == 1)//������ ����
        {
            digitalWrite(Dir1Pin_B, HIGH);//���������� ȸ��
            digitalWrite(Dir2Pin_B, LOW);
            analogWrite(SpeedPin_B, 255);
            delay(1000);

            digitalWrite(Dir1Pin_A, LOW);//�ϰ�
            digitalWrite(Dir2Pin_A, HIGH);
            analogWrite(SpeedPin_A, 2);//(2cm ȸ��)
            delay(1000);

            digitalWrite(Dir1Pin_B, LOW);//�������� ȸ��(����� ȸ��)
            digitalWrite(Dir2Pin_B, HIGH);
            analogWrite(SpeedPin_B, 255);
            delay(1000);
        }
        else//���ʼ���
        {
            digitalWrite(Dir1Pin_B, LOW);//�������� ȸ��
            digitalWrite(Dir2Pin_B, HIGH);
            analogWrite(SpeedPin_B, 255);
            delay(1000);

            digitalWrite(Dir1Pin_A, LOW);//�ϰ�
            digitalWrite(Dir2Pin_A, HIGH);
            analogWrite(SpeedPin_A, 2);//(2cm ȸ��)
            delay(1000);

            digitalWrite(Dir1Pin_B, HIGH);//���������� ȸ��
            digitalWrite(Dir2Pin_B, LOW);
            analogWrite(SpeedPin_B, 255);
            delay(1000);
        }
    else// �ݳ��϶�
    {
        digitalWrite(Dir1Pin_A, LOW);//�ϰ�
        digitalWrite(Dir2Pin_A, HIGH);
        analogWrite(SpeedPin_A, 1);//(2cm ȸ��)
        delay(1000);

        if (direction == 1)//������ �ݳ�
        {
            digitalWrite(Dir1Pin_B, HIGH);//���������� ȸ��
            digitalWrite(Dir2Pin_B, LOW);
            analogWrite(SpeedPin_B, 255);
            delay(1000);

            digitalWrite(Dir1Pin_A, HIGH);//���
            digitalWrite(Dir2Pin_A, LOW);
            analogWrite(SpeedPin_A, 2);//���ڴ� �ٲٸ��(2cm ȸ��)

            digitalWrite(Dir1Pin_B, LOW);//�������� ȸ��
            digitalWrite(Dir2Pin_B, HIGH);
            analogWrite(SpeedPin_B, 255);
            delay(1000);
        }
        else//���ʹݳ�
        {
            digitalWrite(Dir1Pin_B, LOW);//�������� ȸ��
            digitalWrite(Dir2Pin_B, HIGH);
            analogWrite(SpeedPin_B, 255);
            delay(1000);

            digitalWrite(Dir1Pin_A, HIGH);//���
            digitalWrite(Dir2Pin_A, LOW);
            analogWrite(SpeedPin_A, 2);//���ڴ� �ٲٸ��(2cm ȸ��)

            digitalWrite(Dir1Pin_B, HIGH);//���������� ȸ��
            digitalWrite(Dir2Pin_B, LOW);
            analogWrite(SpeedPin_B, 255);
            delay(1000);
        }
    }
    else if (motion == 1)//�ݳ�
    {
        /*stepper1.step(step1 * (-1));//�ϰ�
        delay(1000);
        stepper2.step(direction * step2);
        delay(1000);
        stepper1.step(step1 * 2);
        delay(1000);
        stepper2.step(direction * step2 * (-1));
        delay(1000);*/
        digitalWrite(Dir1Pin_A, LOW);//���������� ȸ��
        digitalWrite(Dir2Pin_A, HIGH);
        analogWrite(SpeedPin_A, 255);
        delay(1000);

        digitalWrite(Dir1Pin_A, HIGH);//�������� ȸ��
        digitalWrite(Dir2Pin_A, LOW);
        analogWrite(SpeedPin_A, 255);
        delay(1000);
    }
}

void Floor_Move(int floor,int *c_floor)
{
    if (floor == 1 && *c_floor != 1)
    {
        /*stepper1.step(-4074); //���ڴ� ��Ȯ�� ������ �����ؾ��Ѵ�.
        *c_floor = 1;*/
        digitalWrite(Dir1Pin_A, HIGH);//���
        digitalWrite(Dir2Pin_A, LOW);
        analogWrite(SpeedPin_A, 255);//���ڴ� �ٲٸ��
    }
    else if (floor == 2 && *c_floor != 2)
    {
        /*stepper1.step(4074); //���ڴ� ��Ȯ�� ������ �����ؾ��Ѵ�.
        *c_floor = 2;*/
        digitalWrite(Dir1Pin_A, LOW);//�ϰ�
        digitalWrite(Dir2Pin_A,HIGH);
        analogWrite(SpeedPin_A, 255);//���ڴ� �ٲٸ��
    }
    
}












#include<Keypad.h>

const byte ROWS = 4;//Ű�е� �κ� ����(Command)
const byte COLS = 3;
char keys[ROWS][COLS] = {
     {'1','2','3'},
     {'4','5','6'},
     {'7','8','9'},
     {'#','0','*'}
};
byte rowPins[ROWS] = { 6,5 };
byte colPins[COLS] = { 4,3,2 };

const int sensorpin1 = A0;//�з¼����� ������ �Ƴ��α� ��(state), �ɹ�ȣ���°� �����ؾ���, ��������
const int sensorpin2 = A1;
const int sensorpin3 = A2;
const int sensorpin4 = A3;
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // Ű�е� �б�
void setup()
{
    Serial.begin(9600);
    //stepper1.setSpeed(200); //���� 1 ���ǵ� ����
    //stepper2.setSpeed(200); //���� 2 ���ǵ� ����
   /* pinMode(Dir1Pin_A, OUTPUT);
    pInMode(Dir2Pin_A, OUTPUT);
    pinMode(SpeedPin_A, OUTPUT);
    pinMode(Dir1Pin_B, OUTPUT);
    pInMode(Dir2Pin_B, OUTPUT);
    pinMode(SpeedPin_B, OUTPUT);*/

}
void loop()
{
    int number = 0, order = 0, state[4] = { 0 }, motion = 0, object = 0;
    State(state);// �з¼��� ��������
    Serial.println("Ű�е� �Է����ּ���.");
    key = kpd.getKey();
    if (key)
        Command(&key, &number, &order);//Ű�е��Է�
    Serial.println("number����");
    Serial.println("number");
    Serial.println("order����");
    Serial.println("order");
    //Route(order, number, state);//����/�ݳ�
  //  Serial.end();//��

}




void Command(char* key, int* number, int* order)
{
    if (*key != 0)
    {
        switch (*key)
        {
        case '6':
            *order = 6;
            break;
        case '1':
            *order = 1;
            *number = 1;
            Serial.println(*key);//��¾��ҰŸ� ������
            break;
        case '2':
            *order = 1;
            *number = 2;
            Serial.println(*key);
            break;
        case '3':
            *order = 1;
            *number = 3;
            Serial.println(*key);
            break;
        case '4':
            *order = 1;
            *number = 4;
            Serial.println(*key);
            break;
        default:
            Serial.println("�߸��� key�� �Է��߽��ϴ�.");
        }
    }
}

void State(int* state)
{
    state[0] = analogRead(sensorpin1);
    state[1] = analogRead(sensorpin2);
    state[2] = analogRead(sensorpin3);
    state[3] = analogRead(sensorpin4);

    /* for (int i = 0;i < 4;i++)
     {
         if (state[i] == 0)
         {
             state[i] = 0;
         }
         else if ((state[i]>=15)&&(state[i]<=100) ) //X���� �з¼��������� �����ؾ���.
         {
             state[i] = 1;
         }
         else if (state[i] > 150)
         {
             state[i] = 2;
         }
     }*/
    Serial.print("ù��°");
    Serial.println(state[0]);
    Serial.print("�ι�°");
    Serial.println(state[1]);
    Serial.print("����°");
    Serial.println(state[2]);
    Serial.print("�׹�°");
    Serial.println(state[3]);
    Serial.print("\n");
    delay(3000);

}
