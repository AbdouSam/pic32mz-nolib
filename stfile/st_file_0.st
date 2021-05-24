PROGRAM moreadvance
  VAR
    m_run AT %IX0.0 : BOOL;
  END_VAR
  VAR
    timvalue : TIME := T#1000ms;
  END_VAR
  VAR
    m_stop AT %IX0.1 : BOOL;
    m_dir AT %IX0.2 : BOOL;
    m_fw AT %QX0.0 : BOOL;
    m_bw AT %QX0.1 : BOOL;
    m_led AT %QX0.2 : BOOL;
  END_VAR
  VAR
    m_state : BOOL;
    TON0 : TON;
    TON1 : TON;
  END_VAR
  VAR_INPUT
    test : BOOL;
  END_VAR
  VAR
    GT20_OUT : BOOL;
  END_VAR

  m_state := NOT(m_stop) AND (m_run OR m_state);
  TON0(IN := m_state AND m_dir, PT := timvalue);
  m_fw := TON0.Q;
  TON1(IN := m_state AND NOT(m_dir), PT := T#1000ms);
  m_bw := TON1.Q;
  GT20_OUT := GT(TON0.ET, TON1.ET);
  m_led := GT20_OUT;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    VAR_GLOBAL
      version : DINT;
    END_VAR
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : moreadvance;
  END_RESOURCE
END_CONFIGURATION
