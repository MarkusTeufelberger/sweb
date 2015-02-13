/**
 * @file Console.h
 */

#ifndef CONSOLE_H__
#define CONSOLE_H__

#include "types.h"
#include <ulist.h>
#include "Mutex.h"

#include "Thread.h"

class Terminal;
extern uint32 boot_completed;
/**
 * @class Console Base Class Console
 */
class Console : public Thread
{
    friend class Terminal;
    friend class ConsoleManager;

  public:

    /**
     * enum Console Foregroundcolors
     */
    enum FOREGROUNDCOLORS
    {
      FG_BLACK = 0,
      FG_BLUE,
      FG_GREEN,
      FG_CYAN,
      FG_RED,
      FG_MAGENTA,
      FG_BROWN,
      FG_WHITE,
      FG_DARK_GREY,
      FG_BRIGHT_BLUE,
      FG_BRIGHT_GREEN,
      FG_BRIGHT_CYAN,
      FG_PINK,
      FG_BRIGHT_MAGENTA,
      FG_YELLOW,
      FG_BRIGHT_WHITE
    };

    /**
     * enum Console Backgroundcolors
     */
    enum BACKGROUNDCOLORS
    {
      BG_BLACK = 0,
      BG_BLUE,
      BG_GREEN,
      BG_CYAN,
      BG_RED,
      BG_MAGENTA,
      BG_BROWN,
      BG_WHITE,
      BG_DARK_GREY,
      BG_BRIGHT_BLUE,
      BG_BRIGHT_GREEN,
      BG_BRIGHT_CYAN,
      BG_PINK,
      BG_BRIGHT_MAGENTA,
      BG_YELLOW,
      BG_BRIGHT_WHITE
    };

    /**
     * Constructor creates a Console Thread
     * @param num_terminals ignored
     * @return Console instance
     */
    Console(uint32 num_terminals);

    Console(uint32 num_terminals, const char *name);

    /**
     * Writes input from the keyboard to the active terminal
     */
    virtual void Run();

    /**
     * Checks if the given key is displayable.
     * @param key the key to check
     * @return true if displayable
     */
    bool isDisplayable(uint32 key);

    /**
     * Destructor
     */
    virtual ~Console()
    {
    }

    /**
     * Returns the number of terminals.
     * @return number of terminals
     */
    uint32 getNumTerminals() const;

    /**
     * Returns a Terminal pointer to the active terminal.
     * @return the Terminal pointer
     */
    Terminal *getActiveTerminal();

    /**
     * Returns the Terminal pointer to the terminal with the given number.
     * @param term the terminal number
     * @return the Terminal pointer
     */
    Terminal *getTerminal(uint32 term);

    /**
     * Sets the terminal with the given number active.
     * Dangerous: will most likely produce a deadlock.
     * @param term the number of the terminal to set active
     */
    void setActiveTerminal(uint32 term);

    /**
     * Acquires the drawing lock.
     */
    void lockConsoleForDrawing();

    /**
     * Releases the drawing lock.
     */
    void unLockConsoleForDrawing();

    /**
     * Checks if all Console locks are free.
     * @return true if all locks are free
     */
    bool areLocksFree()
    {
      return (!boot_completed || (console_lock_.isFree() && locked_for_drawing_ == 0));
    }

  protected:

    /**
     * Handles special non displayable keys:
     * F-keys for switching active terminals
     * displaying threads list
     * backspace
     * @param key the key to handle
     */
    void handleKey(uint32 key);

    virtual void consoleClearScreen() =0;
    virtual uint32 consoleSetCharacter(uint32 const &row, uint32 const&column, uint8 const &character,
                                       uint8 const &state) =0;
    virtual uint32 consoleGetNumRows() const=0;
    virtual uint32 consoleGetNumColumns() const=0;
    virtual void consoleScrollUp() =0;
    virtual void consoleSetForegroundColor(FOREGROUNDCOLORS const &color) =0;
    virtual void consoleSetBackgroundColor(BACKGROUNDCOLORS const &color) =0;

    ustl::list<Terminal *> terminals_;
    Mutex console_lock_;
    Mutex set_active_lock_;
    uint8 locked_for_drawing_;

    uint32 active_terminal_;

};

extern Console* main_console;

#endif
