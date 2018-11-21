/*
 *  Terminal Handler
 */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <algorithm>

#include <iostream>
#include "Term.hpp"

namespace ts {

/* Default Screen Resolution */
#define DEFAULT_WIDTH 80
#define DEFAULT_HEIGHT 24

#define CTRL_BUFSZ 10

/*
 * Terminal Color Codes
 */

/* Forground */
static const char_t kColorDefaultCode[] = "\033[39m";
static const char_t kColorBlackCode[] = "\033[30m";
static const char_t kColorRedCode[] = "\033[31m";
static const char_t kColorGreenCode[] = "\033[32m";
static const char_t kColorYellowCode[] = "\033[33m";
static const char_t kColorBlueCode[] = "\033[34m";
static const char_t kColorMagentaCode[] = "\033[35m";
static const char_t kColorCyanCode[] = "\033[36m";
static const char_t kColorLightGrayCode[] = "\033[37m";
static const char_t kColorDarkGrayCode[] = "\033[90m";
static const char_t kColorLightRedCode[] = "\033[91m";
static const char_t kColorLightGreenCode[] = "\033[92m";
static const char_t kColorLightYellowCode[] = "\033[93m";
static const char_t kColorLightBlueCode[] = "\033[94m";
static const char_t kColorLightMagentaCode[] = "\033[95m";
static const char_t kColorLightCyanCode[] = "\033[96m";
static const char_t kColorWhiteCode[] = "\033[97m";

/* Background */
static const char_t kColorDefaultBackgroundCode[] = "\033[49m";
static const char_t kColorBlackBackgroundCode[] = "\033[40m";
static const char_t kColorRedBackgroundCode[] = "\033[41m";
static const char_t kColorGreenBackgroundCode[] = "\033[42m";
static const char_t kColorYellowBackgroundCode[] = "\033[43m";
static const char_t kColorBlueBackgroundCode[] = "\033[44m";
static const char_t kColorMagentaBackgroundCode[] = "\033[45m";
static const char_t kColorCyanBackgroundCode[] = "\033[46m";
static const char_t kColorLightGrayBackgroundCode[] = "\033[47m";
static const char_t kColorDarkGrayBackgroundCode[] = "\033[100m";
static const char_t kColorLightRedBackgroundCode[] = "\033[101m";
static const char_t kColorLightGreenBackgroundCode[] = "\033[102m";
static const char_t kColorLightYellowBackgroundCode[] = "\033[103m";
static const char_t kColorLightBlueBackgroundCode[] = "\033[104m";
static const char_t kColorLightMagentaBackgroundCode[] = "\033[105m";
static const char_t kColorLightCyanBackgroundCode[] = "\033[106m";
static const char_t kColorWhiteBackgroundCode[] = "\033[107m";

/*
 *  Terminal Mood Codes
 */

static const char_t kMoodBold[] = "\033[1m";
static const char_t kMoodDim[] = "\033[2m";
static const char_t kMoodUnderling[] = "\033[4m";
static const char_t kMoodBlink[] = "\033[5m";
static const char_t kMoodReverse[] = "\033[7m";

static const char_t kMoodBoldReset[] = "\033[21m";
static const char_t kMoodDimReset[] = "\033[22m";
static const char_t kMoodUnderlingReset[] = "\033[24m";
static const char_t kMoodBlinkReset[] = "\033[25m";
static const char_t kMoodReverseReset[] = "\033[27m";

/* Clear All */
static const char_t kResetAll[] = "\033[0m";

using std::cerr;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::unique_ptr;

Term::Term()
    : _fd(0),
      _width(0),
      _height(0),
      _cur_row(0),
      _cur_col(0),
      _forground_color(TermColor::NONE),
      _background_color(TermColor::NONE),
      _text_mood({{TermMood::BLINKING, false},
                  {TermMood::BOLD, false},
                  {TermMood::DIM, false},
                  {TermMood::UNDERLINED, false}}),
      _term_set(false) {
  memset(&_original_term_state, 0, sizeof(struct termios));
}

bool_t Term::Init(fd_t fd) {
  _fd = fd;

  if (_fd < 0) {
    cerr << "Provided invalid file descriptor" << endl;
    return false;
  }

  if (!isatty(this->GetFd())) {
    cerr << "Provided file descriptor is not a terminal" << endl;
    return false;
  }

  if (!this->DetermineTermSize()) {
    cerr << "Couldn't determine size" << endl;
    return false;
  }

  if (!this->InitTerminalState()) {
    cerr << "Failed to initialize terminal state" << endl;
    return false;
  }

  this->Resize();

  this->SetCursorCol(0);
  this->SetCursorRow(0);

  return true;
}

bool_t Term::InitTerminalState(void) {
  struct termios new_state;

  if (tcgetattr(GetFd(), &_original_term_state) < 0) {
    cerr << "Failed to get original terminal state" << endl;
    return false;
  }

  memcpy(&new_state, &_original_term_state, sizeof(struct termios));

  /*
   *  c_iflag - Input flag
   *  c_oflag - Output flag
   *  c_cflag - Control flag
   *  c_lflag - Local flags
   *  c_cc[NCCS] - spacial
   */

  /*
   *  ICANON - Disable canonical.  Allows for character by character
   *      input (canonical mode is line by line).
   *  ECHO - Disable character echo.
   */
  new_state.c_lflag &= ~(ECHO | ICANON);

  /*
   *  VMIN - Minimum characters for a read
   *  VTIME - Minimum deca-seconds before timeout
   *    When both VMIN and VTIME are 0, input is availble immidiately
   *    without any timeout, read will not block without anything read.
   */
  new_state.c_cc[VMIN] = 0;
  new_state.c_cc[VTIME] = 0;

  if (tcsetattr(this->GetFd(), TCSAFLUSH, &new_state) < 0) {
    cerr << "Failed to set terminal attributes" << endl;
    return false;
  }

  _term_set = true;
  return true;
}

bool_t Term::RestoreTerminalState(void) {
  if (!_term_set) {
    /* Terminal isn't set */
    return true;
  }

  if (tcsetattr(this->GetFd(), TCSAFLUSH, &_original_term_state) < 0) {
    return false;
  }

  _term_set = false;
  return true;
}

void Term::SetForgroundColor(TermColor color) {
  _forground_color = color;

  switch (color) {
    case ts::TermColor::BLACK:
      Print(kColorBlackCode);
      break;
    case ts::TermColor::BLUE:
      Print(kColorBlueCode);
      break;
    case ts::TermColor::CYAN:
      Print(kColorCyanCode);
      break;
    case ts::TermColor::DARK_GRAY:
      Print(kColorDarkGrayCode);
      break;
    case ts::TermColor::DEFAULT:
      Print(kColorDefaultCode);
      break;
    case ts::TermColor::GREEN:
      Print(kColorGreenCode);
      break;
    case ts::TermColor::LIGHT_BLUE:
      Print(kColorLightBlueCode);
      break;
    case ts::TermColor::LIGHT_CYAN:
      Print(kColorLightCyanCode);
      break;
    case ts::TermColor::LIGHT_GRAY:
      Print(kColorLightGrayCode);
      break;
    case ts::TermColor::LIGHT_GREEN:
      Print(kColorLightGreenCode);
      break;
    case ts::TermColor::LIGHT_MAGENTA:
      Print(kColorLightMagentaCode);
      break;
    case ts::TermColor::LIGHT_RED:
      Print(kColorLightRedCode);
      break;
    case ts::TermColor::LIGHT_YELLOW:
      Print(kColorLightYellowCode);
      break;
      Print(kColorMagentaCode);
      break;
    case ts::TermColor::RED:
      Print(kColorRedCode);
      break;
    case ts::TermColor::WHITE:
      Print(kColorWhiteCode);
      break;
    case ts::TermColor::YELLOW:
      Print(kColorYellowCode);
      break;
  }
}

void Term::SetBackgroundColor(TermColor color) {
  _background_color = color;

  switch (color) {
    case ts::TermColor::BLACK:
      Print(kColorBlackBackgroundCode);
      break;
    case ts::TermColor::BLUE:
      Print(kColorBlueBackgroundCode);
      break;
    case ts::TermColor::CYAN:
      Print(kColorCyanBackgroundCode);
      break;
    case ts::TermColor::DARK_GRAY:
      Print(kColorDarkGrayBackgroundCode);
      break;
    case ts::TermColor::DEFAULT:
      Print(kColorDefaultBackgroundCode);
      break;
    case ts::TermColor::GREEN:
      Print(kColorGreenBackgroundCode);
      break;
    case ts::TermColor::LIGHT_BLUE:
      Print(kColorLightBlueBackgroundCode);
      break;
    case ts::TermColor::LIGHT_CYAN:
      Print(kColorLightCyanBackgroundCode);
      break;
    case ts::TermColor::LIGHT_GRAY:
      Print(kColorLightGrayBackgroundCode);
      break;
    case ts::TermColor::LIGHT_GREEN:
      Print(kColorLightGreenBackgroundCode);
      break;
    case ts::TermColor::LIGHT_MAGENTA:
      Print(kColorLightMagentaBackgroundCode);
      break;
    case ts::TermColor::LIGHT_RED:
      Print(kColorLightRedBackgroundCode);
      break;
    case ts::TermColor::LIGHT_YELLOW:
      Print(kColorLightYellowBackgroundCode);
      break;
    case ts::TermColor::MAGENTA:
      Print(kColorMagentaBackgroundCode);
      break;
    case ts::TermColor::RED:
      Print(kColorRedBackgroundCode);
      break;
    case ts::TermColor::WHITE:
      Print(kColorWhiteBackgroundCode);
      break;
    case ts::TermColor::YELLOW:
      Print(kColorYellowBackgroundCode);
      break;
  }
}

void Term::SetTextMood(TermMood mood, bool_t state) {
  _text_mood[mood] = state;

  if (state) {
    switch (mood) {
      case ts::TermMood::BLINKING:
        Print(kMoodBlink);
        break;
      case ts::TermMood::BOLD:
        Print(kMoodBold);
        break;
      case ts::TermMood::DIM:
        Print(kMoodDim);
        break;
      case ts::TermMood::UNDERLINED:
        Print(kMoodUnderling);
        break;
    }
  } else {
    switch (mood) {
      case ts::TermMood::BLINKING:
        Print(kMoodBlinkReset);
        break;
      case ts::TermMood::BOLD:
        Print(kMoodBoldReset);
        break;
      case ts::TermMood::DIM:
        Print(kMoodDimReset);
        break;
      case ts::TermMood::UNDERLINED:
        Print(kMoodUnderlingReset);
        break;
    }
  }
}

TermColor Term::GetForgroundColor() const { return _forground_color; }

TermColor Term::GetBackgroundColor() const { return _background_color; }

bool_t Term::GetTextMood(TermMood mood) const { return _text_mood.at(mood); }

bool_t Term::DetermineTermSize(void) {
  int32_t res;
  struct winsize ws;

  do {
    res = ioctl(this->GetFd(), TIOCGWINSZ, &ws);
  } while (res < 0 && (EAGAIN == errno || EINTR == errno));

  if (res < 0) {
    switch (errno) {
      case EINVAL:
        cerr << "Invalid ioctl() argument" << endl;
        break;
      case ENOTTY:
        cerr << "Not a TTY" << endl;
        break;
      case EPERM:
        cerr << "Invalid permissions" << endl;
        break;
      default:
        cerr << "Unknown ioctl() error" << endl;
        break;
    }
    return false;
  }

  _width = ws.ws_col;
  _height = ws.ws_row;

  if (!_width) {
    _width = DEFAULT_WIDTH;
  }
  _width--;

  if (!_height) {
    _height = DEFAULT_HEIGHT;
  }

  return true;
}

fd_t Term::GetFd(void) const { return _fd; }

void Term::Print(char_t const* buf) {
  int32_t res;

  do {
    res = write(GetFd(), buf, strlen(buf));
  } while (res < 0 && (EAGAIN == errno || EINTR == errno));
}

Term::~Term() {
  SetCursorCol(0);
  SetCursorRow(0);
  RestoreTerminalState();
}

shared_ptr<Term> Term::Create(fd_t term_fd) {
  shared_ptr<Term> term(new Term());

  if (!term->Init(term_fd)) {
    cerr << "Failed to initialize terminal" << endl;
    term.reset();
  }

  return term;
}

/* Resolution State */
uint32_t Term::GetWidth(void) const { return _width; }

uint32_t Term::GetHeight(void) const { return _height; }

void Term::Resize(void) {
  uint32_t r, c;

  for (r = 0; r < GetHeight(); r++) {
    Print("\n");
    for (c = 0; c < GetWidth(); c++) {
      Print(" ");
    }
  }
  _cur_row = GetHeight();
  _cur_col = GetWidth();
}

int32_t Term::GetCursorRow(void) const { return _cur_row; }

int32_t Term::GetCursorCol(void) const { return _cur_col; }

void Term::SetCursorRow(int32_t row) {
  if (GetCursorRow() == row) return;

  char_t ctrlbuf[CTRL_BUFSZ];

  uint32_t r = std::clamp(row, 0, (int)GetHeight());

  memset(ctrlbuf, 0, CTRL_BUFSZ);
  uint32_t curClamped = std::clamp(GetCursorRow(), 0, (int)GetHeight() - 1);


  if (curClamped < r) {
    /* Move Down */
    sprintf(ctrlbuf, "\033[%dB", (unsigned short)r - curClamped);
  } else {
    /* Move Up */
    sprintf(ctrlbuf, "\033[%dA", (unsigned short)curClamped - r);
  }

  _cur_row = row;

  Print(ctrlbuf);
}

void Term::SetCursorCol(int32_t col) {
  if (GetCursorCol() == col) return;

  char_t ctrlbuf[CTRL_BUFSZ];

  uint32_t c = ::std::clamp(col, 0, (int)GetWidth() - 1);

  memset(ctrlbuf, 0, CTRL_BUFSZ);

  uint32_t curClamped = std::clamp(GetCursorCol(), 0, (int)GetWidth());

  if (curClamped < c) {
    /* Move Right */
    sprintf(ctrlbuf, "\033[%dC", (unsigned short)c - curClamped);
  } else {
    /* Move Left */
    sprintf(ctrlbuf, "\033[%dD", (unsigned short)curClamped - c);
  }

  _cur_col = col;

  Print(ctrlbuf);
}

void Term::MoveCursorLeft(int32_t dis) {
  if (dis < 0) {
    MoveCursorRight(-dis);
    return;
  }
  SetCursorCol(GetCursorCol() - dis);
}

void Term::MoveCursorRight(int32_t dis) {
  if (dis < 0) {
    MoveCursorLeft(-dis);
    return;
  }
  SetCursorCol(GetCursorCol() + dis);
}

void Term::MoveCursorUp(int32_t dis) {
  if (dis < 0) {
    MoveCursorDown(-dis);
    return;
  }
  SetCursorRow(GetCursorRow() - dis);
}

void Term::MoveCursorDown(int32_t dis) {
  if (dis < 0) {
    MoveCursorUp(-dis);
    return;
  }
  SetCursorRow(GetCursorRow() + dis);
}

void Term::PutChar(char_t c) {
  char_t cbuf[2];
  if (!isprint(c)) return;

  if (_cur_col < 0 || _cur_col >= _width || _cur_row < 0 || _cur_row >= _height)
    return;

  memset(cbuf, 0, sizeof(cbuf));

  if (GetCursorCol() == GetWidth()) {
    if ((GetCursorRow() + 1) == GetHeight()) {
      SetCursorRow(0);
    } else {
      MoveCursorDown(1);
    }
    SetCursorCol(0);
  }

  cbuf[0] = c;
  Print(cbuf);
  _cur_col++;
}

void Term::PutString(::std::string const& s) {
  for (auto const& c : s) {
    PutChar(c);
  }
}

void Term::ClearTerm(void) {
  uint32_t old_row, old_col;
  uint32_t r, c;
  TermColor fgColor = _forground_color;
  TermColor bgColor = _background_color;
  ::std::unordered_map<TermMood, bool_t> mood = _text_mood;

  ClearColors();
  ClearMood();

  /* Save cursor position */
  old_row = _cur_row;
  old_col = _cur_col;
  SetCursorCol(0);
  SetCursorRow(0);

  for (r = 0; r < GetHeight(); r++) {
    for (c = 0; c < GetWidth(); c++) {
      // TODO make this better
      PutChar(' ');
    }
    SetCursorCol(0);
    MoveCursorDown(1);
  }

  SetForgroundColor(fgColor);
  SetBackgroundColor(bgColor);
  _text_mood = mood;

  SetCursorCol(old_col);
  SetCursorRow(old_row);
}

void Term::ClearColors() {
  SetForgroundColor(TermColor::DEFAULT);
  SetBackgroundColor(TermColor::DEFAULT);
}

void Term::ClearMood() {
  SetTextMood(TermMood::BLINKING, false);
  SetTextMood(TermMood::BOLD, false);
  SetTextMood(TermMood::DIM, false);
  SetTextMood(TermMood::UNDERLINED, false);
}

void Term::SetCursorVisible(bool_t visible) {
  if (visible) {
    auto a = system("setterm -cursor on");
  } else {
    auto a = system("setterm -cursor off");
  }
}
}  // namespace ts
