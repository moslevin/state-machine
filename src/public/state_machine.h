/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2017 - 2018 m0slevin, all rights reserved.
See license.txt for more information
===========================================================================*/
/**
    @file state_machine.h
    @brief Implements a generic and extensible state-machine framework
*/

#pragma once

#include <stdint.h>

namespace Mark3
{
//---------------------------------------------------------------------------
/**
 * Possible state handler return codes
 */
enum class StateReturn : uint8_t{
    ok,        //!< Handler returned successfully
    unhandled, //!< Event not handled by state handler
    transition //!< Event handling resulted in state transition
};

//---------------------------------------------------------------------------
/**
 * Possible state machine operations
 */
enum class StateOpcode : uint8_t {
    returned,     //!< Event was consumed, no further operations
    run,  //!< Run the state machine
    push, //!< Push the current state to the stack
    pop,  //!< Pop the current state from the stack
    transition, //!< Transition to a new state within the current level
    unhandled, //!< Event was not handled by any handler in the stack
};

//---------------------------------------------------------------------------
// Forward declaration
class StateMachine;

//---------------------------------------------------------------------------
// Function pointer type used for implementing state entry/exit functions
typedef void (*StateChangeHandler_t)(StateMachine* pclSM_);

// Function pointer type used for implementing active state handlers
typedef StateReturn (*StateHandler_t)(StateMachine* pclSM_, const void* pvEvent_);

// Define error types that are propagated to error handler functions
enum class StateErrorType : uint8_t {
    ambiguous_operation,
    invalid_state,
    state_stack_overflow,
    state_stack_underflow
};

// Struct that defines error event data
typedef struct {
    StateErrorType eType;
    union {
        struct {
            StateOpcode eInitialOp;
            StateOpcode eAmbiguousOp;
            uint16_t u16StartState;
            uint16_t u16CurrentState;
        } ambiguousOperation;
        struct {
            uint16_t u16InvalidState;
        } invalidState;
        struct {
            uint16_t u16FailedState;
        } stateStackOverflow;
    };
} StateErrorData_t;

typedef void (*StateErrorHandler_t)(StateMachine* pclSM_, const StateErrorData_t* pstOpData_);

//---------------------------------------------------------------------------
// State structure definition
typedef struct {
    StateChangeHandler_t pfEntry; //!< (optional) Function called on state entry
    StateHandler_t       pfRun;   //!< Function called when running state
    StateChangeHandler_t pfExit;  //!< (optional) Function called on state exit
} State_t;

//---------------------------------------------------------------------------
#define MAX_STATE_STACK_DEPTH (8)

//---------------------------------------------------------------------------
/**
 * @brief The StateMachine class
 *
 * This class implements a state machine using a simple stack-based VM.
 * StateMachine objects are assigned a state-table (a linear array of states,
 * containing functions to be run on entry/exit/normal execution of a
 * state), and a context (object with common data used by all states).
 *
 * Events are delivered to the state machine via the HandleEvent method,
 * which are then passed to the current state's execution handler for
 * processing.  The events may cause the state machine to transition
 * between states, or even transition to substates within a state by
 * pushing and popping states from the state machine's stack.
 *
 */
class StateMachine
{
public:
    StateMachine();
    /**
     * @brief SetStates
     *
     * Set the state table on which the object executes.  This table
     * must exist for the lifespan of the state machine.  Must only
     * be set once per instance.
     *
     * @param pstStates_ pointer to the state machine table
     * @param u16StateCount_ number of states held in the table
     * @return true on success, false if called multiple times
     */
    bool SetStates(const State_t* pstStates_, uint16_t u16StateCount_);

    /**
     * @brief SetContext
     *
     * Set a context object held by the state machine, which is passed
     * to each state handler.  This typically contains the common data
     * used by the various objects in the system.  This object must
     * exist for the lifetime of the state machine instance.
     *
     * @param pvContext_ Pointer to the context to set
     */
    void SetContext(void* pvContext_);

    /**
     * @brief GetContext
     *
     * Retrieve a pointer to the context object held by the state machine.
     * @sa SetContext
     *
     * @return Pointer to the context object, or NULL if no context set.
     */
    void* GetContext();

    /**
     * @brief Begin
     *
     * Initialize the state machine by resetting the stack and entering the
     * first state in the table.  Requires that SetStates has been previously
     * called.
     *
     * @return true if successfully initialized, false otherwise
     *
     * @sa SetStates
     */
    bool Begin();

    /**
     * @brief HandleEvent
     *
     * Pass an object containing stimulus to the state machine for processing.
     *
     * @param pvEvent_ Stimulus object, which the state machine interprets
     * and processes.
     */
    StateReturn HandleEvent(const void* pvEvent_);

    /**
     * @brief PushState
     *
     * Push the current state to the stack, and enter a new state specified by the
     * index in the state table.
     *
     * @param u16StateIdx_ Index of the new state to be run
     * @return true on success, false if operation is ambiguous (i.e. other transitions
     * or push/pop operations take place within the same handler call)
     */
    bool PushState(uint16_t u16StateIdx_);

    /**
     * @brief PopState
     *
     * Exit out of the current state, and resume execution of the state machine at the
     * state held in the state machine VM's stack.
     *
     * @return true on success, false if operation is ambiguous (i.e. other transitions
     * or push/pop operations take place within the same context)
     */
    bool PopState();

    /**
     * @brief TransitionState
     *
     * Transition the execution of the state machine from its current state to the state
     * specified by its index in the state table
     *
     * @param u16StateIdx_ Index corresponding to the state to be entered
     * @return true on success, false if operation is ambiguous (i.e. other transitions
     * or push/pop operations take place within the same context)
     */
    bool TransitionState(uint16_t u16StateIdx_);

    /**
     * @brief GetCurrentState
     *
     * Retrieve the current state being handled in the state machine
     *
     * @return index of the current state machine's running state
     */
    uint16_t GetCurrentState();

    /**
     * @brief GetStackDepth
     *
     * Retrieve the current state machine's stack depth (number of
     * states "pushed")
     *
     * @return current stack depth
     */
    uint16_t GetStackDepth();

    /**
     * @brief SetErrorHandler
     *
     * Register an error handler function, invoked when a fatal
     * state machine error is encountered.
     *
     * @param pfHandler_ State error-handler function pointer
     */
    void SetErrorHandler(StateErrorHandler_t pfHandler_);

private:
    /**
     * @brief SetOpcode
     *
     * Set the next pending opcode for the state machine
     *
     * @param eOpcode_ next opcode to run
     * @return true on success, false if opcode already pending
     */
    bool SetOpcode(StateOpcode eOpcode_);

    /**
     * @brief GetOpcode
     *
     * Retrieve the next opcode to run from the state machine
     *
     * @param peOpcode_ [out] opcode read from the state machine
     * @return true if an opcode was a returned, false if no opcode pending
     */
    bool GetOpcode(StateOpcode* peOpcode_);

    bool m_bStatesSet; //!< Wheter or not states are configured

    StateOpcode   m_eOpcode;      //!< Pending state machine

    bool          m_bOpcodeSet;   //!< Indicates the state machine has a pending opcode
    uint16_t      m_u16NextState; //!< Next state to run

    void*          m_pvContext;     //!< User context, passed in via SetContext()
    const State_t* m_pstStateList;  //!< Pointer to the state handler array used by this state machine
    uint16_t       m_u16StateCount; //!< Number of states in the state array

    uint16_t m_u16StackDepth;                         //!< Current stack level in the state machine
    uint16_t m_au16StateStack[MAX_STATE_STACK_DEPTH]; //!< State stack

    StateErrorHandler_t m_pfErrorHandler;    //!< Function called on state machine ambiguity.
    uint16_t m_u16OpSetState;
};
} // namespace Mark3
