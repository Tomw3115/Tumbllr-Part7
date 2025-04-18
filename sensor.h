// Contents of file sensor.h

class IR
{
  public:
    void Pin_init();
    void Check();
    void Send();
    
  public:
    int left_is_obstacle;
    int right_is_obstacle;

  private:
    #define RECV_PIN 9
    #define IR_SEND_PIN 9
    #define LEFT_RECEIVE_PIN A0
    #define RIGHT_RECEIVE_PIN A1
    #define If_IR_TRIGGERED (IR.left_is_obstacle || IR.right_is_obstacle)
};

class Ultrasonic
  {
  public:
    void Pin_init();
    void Get_Distance();
    float Check();
    static void Distance_Measure();
  public:
    static char measure_flag;
    static unsigned long measure_prev_time;
    unsigned long get_distance_prev_time;
    static double distance_value;
    unsigned long ir_send_time;
  private:
    #define ECHO_PIN 17
    #define TRIG_PIN 11
    #define Distance_MIN 3
    #define Distance_MAX 35
    #define DISTANCE_JUDGEMENT (distance_value > Distance_MIN && distance_value < Distance_MAX)
};