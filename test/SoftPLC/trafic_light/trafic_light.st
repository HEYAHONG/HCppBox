PROGRAM Trafic_light
  VAR
    TOF0 : TOF;
    TOF1 : TOF;
    TOF2 : TOF;
  END_VAR
  VAR
    O_LEDRed AT %QX0.0 : BOOL;
    O_LEDOrange AT %QX0.1 : BOOL;
    O_LEDGreen AT %QX0.2 : BOOL;
    First_cycle AT %QX0.3 : BOOL := TRUE;
  END_VAR
  VAR
    F_TRIG0 : F_TRIG;
    F_TRIG1 : F_TRIG;
    F_TRIG2 : F_TRIG;
    _TMP_OR67_OUT : BOOL;
  END_VAR

  F_TRIG0(CLK := O_LEDOrange);
  _TMP_OR67_OUT := OR(F_TRIG0.Q, First_cycle);
  TOF0(IN := _TMP_OR67_OUT, PT := T#6s);
  O_LEDRed := TOF0.Q;
  F_TRIG1(CLK := O_LEDGreen);
  TOF1(IN := F_TRIG1.Q, PT := T#4s);
  O_LEDOrange := TOF1.Q;
  F_TRIG2(CLK := O_LEDRed);
  TOF2(IN := F_TRIG2.Q, PT := T#6s);
  O_LEDGreen := TOF2.Q;
  First_cycle := FALSE;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : Trafic_light;
  END_RESOURCE
END_CONFIGURATION

