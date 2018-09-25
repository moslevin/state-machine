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
    @file state_machine.cpp
    @brief Implements a generic and extensible state-machine framework
*/
#include "state_machine.h"

namespace Mark3
{
StateMachine::StateMachine()
    : m_bStatesSet{false}
    , m_bOpcodeSet{false}
    , m_u16NextState{0}
    , m_pstStateList{nullptr}
    , m_pfErrorHandler{nullptr}
{
}
//---------------------------------------------------------------------------
bool StateMachine::SetStates(const State_t* pstStates_, uint16_t u16StateCount_)
{
    if (m_bStatesSet || (!pstStates_) || (0 == u16StateCount_)) {
        return false;
    }

    m_bStatesSet = true;
    m_u16StateCount = u16StateCount_;
    m_pstStateList  = pstStates_;
    return true;
}

//---------------------------------------------------------------------------
void StateMachine::SetContext(void* pvContext_)
{
    m_pvContext = pvContext_;
}

//---------------------------------------------------------------------------
void* StateMachine::GetContext()
{
    return m_pvContext;
}

//---------------------------------------------------------------------------
bool StateMachine::Begin()
{
    if (!m_bStatesSet) {
        return false;
    }

    m_u16StackDepth     = 1;
    m_bOpcodeSet        = false;
    m_au16StateStack[0] = 0;

    if (m_pstStateList[0].pfEntry) {
        m_pstStateList[0].pfEntry(this);
    }
    return true;
}

//---------------------------------------------------------------------------
bool StateMachine::PushState(uint16_t u16StateIdx_)
{
    if (u16StateIdx_ >= m_u16StateCount) {
        if (m_pfErrorHandler != nullptr) {
            StateErrorData_t stError;
            stError.eType = StateErrorType::invalid_state;
            stError.invalidState.u16InvalidState = u16StateIdx_;
            m_pfErrorHandler(this, &stError);
        }
        return false;
    }

    if (m_u16StackDepth >= MAX_STATE_STACK_DEPTH) {
        if (m_pfErrorHandler != nullptr) {
            StateErrorData_t stError;
            stError.eType = StateErrorType::state_stack_overflow;
            stError.stateStackOverflow.u16FailedState = u16StateIdx_;
            m_pfErrorHandler(this, &stError);
        }
        return false;
    }

    if (SetOpcode(StateOpcode::push)) {
        m_u16NextState = u16StateIdx_;
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool StateMachine::PopState()
{
    if (m_u16StackDepth <= 1) {
        if (m_pfErrorHandler != nullptr) {
            StateErrorData_t stError;
            stError.eType = StateErrorType::state_stack_underflow;
            m_pfErrorHandler(this, &stError);
        }
        return false;
    }
    return SetOpcode(StateOpcode::pop);
}

//---------------------------------------------------------------------------
bool StateMachine::TransitionState(uint16_t u16StateIdx_)
{
    if (u16StateIdx_ >= m_u16StateCount) {
        if (m_pfErrorHandler != nullptr) {
            StateErrorData_t stError;
            stError.eType = StateErrorType::invalid_state;
            stError.invalidState.u16InvalidState = u16StateIdx_;
            m_pfErrorHandler(this, &stError);
        }
        return false;
    }

    if (SetOpcode(StateOpcode::transition)) {
        m_u16NextState = u16StateIdx_;
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
void StateMachine::SetErrorHandler(StateErrorHandler_t pfHandler_)
{
    m_pfErrorHandler = pfHandler_;
}

//---------------------------------------------------------------------------
bool StateMachine::SetOpcode(StateOpcode eOpcode_)
{
    if (m_bOpcodeSet) {
        // If an opcode is already set, this indicates an ambiguity in the
        // state machine (i.e. multiple state machine transitions attempted
        // in a single path
        if (m_pfErrorHandler != nullptr) {
            StateErrorData_t stError;
            stError.ambiguousOperation.eAmbiguousOp = eOpcode_;
            stError.ambiguousOperation.eInitialOp = m_eOpcode;
            stError.ambiguousOperation.u16CurrentState = GetCurrentState();
            m_pfErrorHandler(this, &stError);
        }
        return false;
    }
    m_bOpcodeSet = true;
    m_eOpcode    = eOpcode_;
    m_u16OpSetState = GetCurrentState();
    return true;
}

//---------------------------------------------------------------------------
bool StateMachine::GetOpcode(StateOpcode* peOpcode_)
{
    if (m_bOpcodeSet) {
        *peOpcode_   = m_eOpcode;
        m_bOpcodeSet = false;
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
uint16_t StateMachine::GetCurrentState()
{
    return m_au16StateStack[m_u16StackDepth - 1];
}

//---------------------------------------------------------------------------
uint16_t StateMachine::GetStackDepth()
{
    return m_u16StackDepth;
}

//---------------------------------------------------------------------------
StateReturn StateMachine::HandleEvent(const void* pvEvent_)
{
    auto u16StackPtr = m_u16StackDepth;
    auto bDone       = false;
    SetOpcode(StateOpcode::run);
    auto eReturnCode = StateReturn::ok;

    while (!bDone) {
        uint16_t u16State = m_au16StateStack[u16StackPtr - 1];

        // Handle the individual statemachine opcodes.
        StateOpcode eOpcode;
        GetOpcode(&eOpcode);
        switch (eOpcode) {
            case StateOpcode::returned: {
                // Done processing the event
                bDone = true;
            } break;
            case StateOpcode::unhandled: {
                eReturnCode = StateReturn::unhandled;
                bDone = true;
            } break;
            case StateOpcode::run: {
                // Must have a run handler...
                auto eResult = m_pstStateList[u16State].pfRun(this, pvEvent_);
                if (eResult == StateReturn::unhandled) {
                    if (u16StackPtr > 1) {
                        u16StackPtr--;
                        SetOpcode(StateOpcode::run);
                    } else {
                        SetOpcode(StateOpcode::unhandled);
                    }
                } else if (eResult == StateReturn::ok) {
                    SetOpcode(StateOpcode::returned);
                }
                // Implicit - if eResult == STATE_RETURN_TRANSITION, then
                // the m_eOpcode value will have been set before the handler returned, and the
                // m_u16NextState variable also set.

            } break;
            case StateOpcode::push: {
                SetOpcode(StateOpcode::returned);
                eReturnCode = StateReturn::ok;

                while (m_u16StackDepth != u16StackPtr) {
                    uint16_t u16TempState = m_au16StateStack[m_u16StackDepth - 1];
                    m_u16StackDepth--;
                    if (m_pstStateList[u16TempState].pfExit) {
                        m_pstStateList[u16TempState].pfExit(this);
                    }
                }

                if (m_pstStateList[m_u16NextState].pfEntry) {
                    m_pstStateList[m_u16NextState].pfEntry(this);
                }
                m_au16StateStack[m_u16StackDepth] = m_u16NextState;
                m_u16StackDepth++;                
            } break;
            case StateOpcode::pop: {
                SetOpcode(StateOpcode::returned);
                eReturnCode = StateReturn::ok;

                while (m_u16StackDepth != u16StackPtr) {
                    uint16_t u16TempState = m_au16StateStack[m_u16StackDepth - 1];
                    m_u16StackDepth--;
                    if (m_pstStateList[u16TempState].pfExit) {
                        m_pstStateList[u16TempState].pfExit(this);
                    }
                }

                uint16_t u16TempState = m_au16StateStack[m_u16StackDepth - 1];
                m_u16StackDepth--;
                if (m_pstStateList[u16TempState].pfExit) {
                    m_pstStateList[u16TempState].pfExit(this);
                }

            } break;
            case StateOpcode::transition: {
                while (m_u16StackDepth != u16StackPtr) {
                    uint16_t u16TempState = m_au16StateStack[m_u16StackDepth - 1];
                    m_u16StackDepth--;
                    if (m_pstStateList[u16TempState].pfExit) {
                        m_pstStateList[u16TempState].pfExit(this);
                    }
                }

                if (m_pstStateList[u16State].pfExit) {
                    m_pstStateList[u16State].pfExit(this);
                }
                if (m_pstStateList[m_u16NextState].pfEntry) {
                    m_pstStateList[m_u16NextState].pfEntry(this);
                }                
                m_au16StateStack[m_u16StackDepth - 1] = m_u16NextState;

                bDone = true;
                eReturnCode = StateReturn::transition;
            } break;
            default: break;
        }
    }
    return eReturnCode;
}
} // namespace Mark3
