class MyStepper{
  public:
    MyStepper(int, int);
    void go(int, int, int); // speed, direction, steps
  private: 
    int pinPul, pinDir;
};
