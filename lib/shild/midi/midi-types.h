#pragma once
#include <core.h>

enum MIDI_Message : uint8_t {
  // Cannel message

  NoteOff = 0x80,
  NoteOn = 0x90,
  KeyPressure = 0xA0,
  ControlChange = 0xB0,
  ProgramChange = 0xC0,
  ChannelPressure = 0xD0,
  PitchBend = 0xE0,

  // SysEx

  TimeCode = 0xF1,
  SongPositionPointer = 0xF2,
  SongSelect = 0xF3,
  TuneRequest = 0xF6,
  End = 0xF7,

  // Real time

  TimingClock = 0xF8,
  Tick = 0xF9,
  Start = 0xFA,
  Continue = 0xFB,
  Stop = 0xFC,
  ActiveSensing = 0xFE,
  SystemReset = 0xFF
};

enum MIDI_Control : uint8_t {
  BankSelect = 0,
  ModulationWheel = 1,
  BreathController = 2,

  FootController = 4,
  PortamentoTime = 5,
  DataEntryMSB = 6,
  ChannelVolume = 7,
  Balance = 8,

  Pan = 10,
  ExpressionController = 11,
  EffectControl1 = 12,
  EffectControl2 = 13,

  GeneralPurposeController1 = 16,
  GeneralPurposeController2 = 17,
  GeneralPurposeController3 = 18,
  GeneralPurposeController4 = 19,

  BankSelectLSB = 32,
  ModulationWheelLSB = 33,
  BreathControllerLSB = 34,

  FootControllerLSB = 36,
  PortamentoTimeLSB = 37,
  DataEntryLSB = 38,
  ChannelVolumeLSB = 39,
  BalanceLSB = 40,

  PanLSB = 42,
  ExpressionControllerLSB = 43,
  EffectControl1LSB = 44,
  EffectControl2LSB = 45,

  // Switches ----------------------------------------------------------------
  Sustain = 64,
  Portamento = 65,
  Sostenuto = 66,
  SoftPedal = 67,
  Legato = 68,
  Hold = 69,

  // Low resolution continuous controllers -----------------------------------
  SoundController1 = 70,
  SoundController2 = 71,
  SoundController3 = 72,
  SoundController4 = 73,
  SoundController5 = 74,
  SoundController6 = 75,
  SoundController7 = 76,
  SoundController8 = 77,
  SoundController9 = 78,
  SoundController10 = 79,
  GeneralPurposeController5 = 80,
  GeneralPurposeController6 = 81,
  GeneralPurposeController7 = 82,
  GeneralPurposeController8 = 83,
  PortamentoControl = 84,

  Effects1 = 91,
  Effects2 = 92,
  Effects3 = 93,
  Effects4 = 94,
  Effects5 = 95,
  DataIncrement = 96,
  DataDecrement = 97,
  NRPNLSB = 98,
  NRPNMSB = 99,
  RPNLSB = 100,
  RPNMSB = 101,

  // Channel Mode messages ---------------------------------------------------
  AllSoundOff = 120,
  ResetAllControllers = 121,
  LocalControl = 122,
  AllNotesOff = 123,
  OmniModeOff = 124,
  OmniModeOn = 125,
  MonoModeOn = 126,
  PolyModeOn = 127
};
