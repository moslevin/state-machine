#include "state_machine.h"
#include "mark3.h"
#include "unit_test.h"
#include "ut_platform.h"

namespace
{
using namespace Mark3;

enum class TestEventCode : uint8_t {
    next_state,
    prev_state,
    push_to_a,
    push_to_b,
    push_to_c,
    push_to_d,
    push_to_e,
    pop,
    jump_to_a,
    jump_to_b,
    jump_to_c,
    jump_to_d,
    jump_to_e,
    handle_in_a,
    handle_in_b,
    handle_in_c,
    handle_in_d,
    handle_in_e,
    nested_jump,
    ambiguous,
    unhandled,
};

enum class TestStateIndex : uint8_t {
    a,
    b,
    c,
    d,
    e
};

typedef struct {
    TestEventCode eEventCode;
    // Specific source/destination states for nested jumping tests
    TestStateIndex eJumpSource;
    TestStateIndex eJumpDest;
} TestEvent_t;

//---------------------------------------------------------------------------
void aEntry(StateMachine* pclSM_) {
    }
StateReturn aRun(StateMachine* pclSM_, const void* pvEvent_) {
    auto testEvent = reinterpret_cast<const TestEvent_t*>(pvEvent_);

    switch (testEvent->eEventCode) {
    case TestEventCode::ambiguous: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::a);
        pclSM_->TransitionState((uint16_t)TestStateIndex::b);
        return StateReturn::transition;
    } break;
    case TestEventCode::next_state: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::b);
        return StateReturn::transition;
    } break;
    case TestEventCode::prev_state: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::e);
        return StateReturn::transition;
    } break;
    case TestEventCode::push_to_a: {
        pclSM_->PushState((uint16_t)TestStateIndex::a);
    } break;
    case TestEventCode::push_to_b: {
        pclSM_->PushState((uint16_t)TestStateIndex::b);
    } break;
    case TestEventCode::push_to_c: {
        pclSM_->PushState((uint16_t)TestStateIndex::c);
    } break;
    case TestEventCode::push_to_d: {
        pclSM_->PushState((uint16_t)TestStateIndex::d);
    } break;
    case TestEventCode::push_to_e: {
        pclSM_->PushState((uint16_t)TestStateIndex::e);
    } break;
    case TestEventCode::pop: {
        pclSM_->PopState();
    } break;
    case TestEventCode::nested_jump: {
        if (testEvent->eJumpSource == TestStateIndex::a) {
            pclSM_->TransitionState((uint16_t)testEvent->eJumpDest);
        } else {
            return StateReturn::unhandled;
        }
    } break;
    case TestEventCode::jump_to_a: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::a);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_b: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::b);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_c: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::c);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_d: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::d);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_e: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::e);
        return StateReturn::transition;
    } break;
    case TestEventCode::handle_in_a: {
        return StateReturn::ok;
    } break;
    case TestEventCode::handle_in_b:
    case TestEventCode::handle_in_c:
    case TestEventCode::handle_in_d:
    case TestEventCode::handle_in_e:
    default:
        return StateReturn::unhandled;
    }
    return StateReturn::ok;
}
void aExit(StateMachine* pclSM_) {
}

//---------------------------------------------------------------------------
void bEntry(StateMachine* pclSM_) {
}

StateReturn bRun(StateMachine* pclSM_, const void* pvEvent_) {
    auto testEvent = reinterpret_cast<const TestEvent_t*>(pvEvent_);

    switch (testEvent->eEventCode) {
    case TestEventCode::ambiguous: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::a);
        pclSM_->TransitionState((uint16_t)TestStateIndex::b);
        return StateReturn::transition;
    } break;
    case TestEventCode::next_state: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::c);
        return StateReturn::transition;
    } break;
    case TestEventCode::prev_state: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::a);
        return StateReturn::transition;
    } break;
    case TestEventCode::push_to_a: {
        pclSM_->PushState((uint16_t)TestStateIndex::a);
    } break;
    case TestEventCode::push_to_b: {
        pclSM_->PushState((uint16_t)TestStateIndex::b);
    } break;
    case TestEventCode::push_to_c: {
        pclSM_->PushState((uint16_t)TestStateIndex::c);
    } break;
    case TestEventCode::push_to_d: {
        pclSM_->PushState((uint16_t)TestStateIndex::d);
    } break;
    case TestEventCode::push_to_e: {
        pclSM_->PushState((uint16_t)TestStateIndex::e);
    } break;
    case TestEventCode::pop: {
        pclSM_->PopState();
    } break;
    case TestEventCode::nested_jump: {
        if (testEvent->eJumpSource == TestStateIndex::b) {
            pclSM_->TransitionState((uint16_t)testEvent->eJumpDest);
        } else {
            return StateReturn::unhandled;
        }
    } break;
    case TestEventCode::jump_to_a: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::a);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_b: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::b);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_c: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::c);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_d: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::d);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_e: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::e);
        return StateReturn::transition;
    } break;
    case TestEventCode::handle_in_b: {
        return StateReturn::ok;
    } break;
    case TestEventCode::handle_in_a:
    case TestEventCode::handle_in_c:
    case TestEventCode::handle_in_d:
    case TestEventCode::handle_in_e:
    default:
        return StateReturn::unhandled;
    }
    return StateReturn::ok;
}
void bExit(StateMachine* pclSM_) {
}

//---------------------------------------------------------------------------
void cEntry(StateMachine* pclSM_) {
}
StateReturn cRun(StateMachine* pclSM_, const void* pvEvent_) {
    auto testEvent = reinterpret_cast<const TestEvent_t*>(pvEvent_);

    switch (testEvent->eEventCode) {
    case TestEventCode::ambiguous: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::a);
        pclSM_->TransitionState((uint16_t)TestStateIndex::b);
        return StateReturn::transition;
    } break;
    case TestEventCode::next_state: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::d);
        return StateReturn::transition;
    } break;
    case TestEventCode::prev_state: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::b);
        return StateReturn::transition;
    } break;
    case TestEventCode::push_to_a: {
        pclSM_->PushState((uint16_t)TestStateIndex::a);
    } break;
    case TestEventCode::push_to_b: {
        pclSM_->PushState((uint16_t)TestStateIndex::b);
    } break;
    case TestEventCode::push_to_c: {
        pclSM_->PushState((uint16_t)TestStateIndex::c);
    } break;
    case TestEventCode::push_to_d: {
        pclSM_->PushState((uint16_t)TestStateIndex::d);
    } break;
    case TestEventCode::push_to_e: {
        pclSM_->PushState((uint16_t)TestStateIndex::e);
    } break;
    case TestEventCode::pop: {
        pclSM_->PopState();
    } break;
    case TestEventCode::nested_jump: {
        if (testEvent->eJumpSource == TestStateIndex::c) {
            pclSM_->TransitionState((uint16_t)testEvent->eJumpDest);
        } else {
            return StateReturn::unhandled;
        }
    } break;
    case TestEventCode::jump_to_a: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::a);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_b: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::b);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_c: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::c);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_d: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::d);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_e: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::e);
        return StateReturn::transition;
    } break;
    case TestEventCode::handle_in_c: {
        return StateReturn::ok;
    } break;
    case TestEventCode::handle_in_a:
    case TestEventCode::handle_in_b:
    case TestEventCode::handle_in_d:
    case TestEventCode::handle_in_e:
    default:
        return StateReturn::unhandled;
    }
    return StateReturn::ok;
}
void cExit(StateMachine* pclSM_) {
}

//---------------------------------------------------------------------------
void dEntry(StateMachine* pclSM_) {
}
StateReturn dRun(StateMachine* pclSM_, const void* pvEvent_) {
    auto testEvent = reinterpret_cast<const TestEvent_t*>(pvEvent_);

    switch (testEvent->eEventCode) {
    case TestEventCode::ambiguous: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::a);
        pclSM_->TransitionState((uint16_t)TestStateIndex::b);
        return StateReturn::transition;
    } break;
    case TestEventCode::next_state: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::e);
        return StateReturn::transition;
    } break;
    case TestEventCode::prev_state: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::c);
        return StateReturn::transition;
    } break;
    case TestEventCode::push_to_a: {
        pclSM_->PushState((uint16_t)TestStateIndex::a);
    } break;
    case TestEventCode::push_to_b: {
        pclSM_->PushState((uint16_t)TestStateIndex::b);
    } break;
    case TestEventCode::push_to_c: {
        pclSM_->PushState((uint16_t)TestStateIndex::c);
    } break;
    case TestEventCode::push_to_d: {
        pclSM_->PushState((uint16_t)TestStateIndex::d);
    } break;
    case TestEventCode::push_to_e: {
        pclSM_->PushState((uint16_t)TestStateIndex::e);
    } break;
    case TestEventCode::pop: {
        pclSM_->PopState();
    } break;
    case TestEventCode::nested_jump: {
        if (testEvent->eJumpSource == TestStateIndex::d) {
            pclSM_->TransitionState((uint16_t)testEvent->eJumpDest);
        } else {
            return StateReturn::unhandled;
        }
    } break;
    case TestEventCode::jump_to_a: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::a);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_b: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::b);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_c: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::c);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_d: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::d);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_e: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::e);
        return StateReturn::transition;
    } break;
    case TestEventCode::handle_in_d:{
        return StateReturn::ok;
    } break;
    case TestEventCode::handle_in_a:
    case TestEventCode::handle_in_b:
    case TestEventCode::handle_in_c:
    case TestEventCode::handle_in_e:
    default:
        return StateReturn::unhandled;
    }
    return StateReturn::ok;
}
void dExit(StateMachine* pclSM_) {
}

//---------------------------------------------------------------------------
void eEntry(StateMachine* pclSM_) {
}
StateReturn eRun(StateMachine* pclSM_, const void* pvEvent_) {
    auto testEvent = reinterpret_cast<const TestEvent_t*>(pvEvent_);

    switch (testEvent->eEventCode) {
    case TestEventCode::ambiguous: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::a);
        pclSM_->TransitionState((uint16_t)TestStateIndex::b);
        return StateReturn::transition;
    } break;
    case TestEventCode::next_state: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::a);
        return StateReturn::transition;
    } break;
    case TestEventCode::prev_state: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::d);
        return StateReturn::transition;
    } break;
    case TestEventCode::push_to_a: {
        pclSM_->PushState((uint16_t)TestStateIndex::a);
    } break;
    case TestEventCode::push_to_b: {
        pclSM_->PushState((uint16_t)TestStateIndex::b);
    } break;
    case TestEventCode::push_to_c: {
        pclSM_->PushState((uint16_t)TestStateIndex::c);
    } break;
    case TestEventCode::push_to_d: {
        pclSM_->PushState((uint16_t)TestStateIndex::d);
    } break;
    case TestEventCode::push_to_e: {
        pclSM_->PushState((uint16_t)TestStateIndex::e);
    } break;
    case TestEventCode::pop: {
        pclSM_->PopState();
    } break;
    case TestEventCode::nested_jump: {
        if (testEvent->eJumpSource == TestStateIndex::e) {
            pclSM_->TransitionState((uint16_t)testEvent->eJumpDest);
        } else {
            return StateReturn::unhandled;
        }
    } break;
    case TestEventCode::jump_to_a: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::a);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_b: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::b);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_c: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::c);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_d: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::d);
        return StateReturn::transition;
    } break;
    case TestEventCode::jump_to_e: {
        pclSM_->TransitionState((uint16_t)TestStateIndex::e);
        return StateReturn::transition;
    } break;
    case TestEventCode::handle_in_e: {
        return StateReturn::ok;
    }
    case TestEventCode::handle_in_a:
    case TestEventCode::handle_in_b:
    case TestEventCode::handle_in_c:
    case TestEventCode::handle_in_d:
    default:
        return StateReturn::unhandled;
    }
    return StateReturn::ok;
}
void eExit(StateMachine* pclSM_) {
}

} // anonymous namespace

//---------------------------------------------------------------------------
namespace Mark3 {
static const State_t testStates[] =
{
    {aEntry, aRun, aExit},
    {bEntry, bRun, bExit},
    {cEntry, cRun, cExit},
    {dEntry, dRun, dExit},
    {eEntry, eRun, eExit}
};

//---------------------------------------------------------------------------
TEST(ut_setstates_pass)
{
    StateMachine sm;

    EXPECT_TRUE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
}

//---------------------------------------------------------------------------
TEST(ut_setstates_fail_2Xinit)
{
    StateMachine sm;

    EXPECT_TRUE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
    EXPECT_FALSE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
}

//---------------------------------------------------------------------------
TEST(ut_setstates_fail_nullstates)
{
    StateMachine sm;

    EXPECT_FALSE(sm.SetStates(nullptr, sizeof(testStates)/sizeof(State_t)));
}

//---------------------------------------------------------------------------
TEST(ut_setstates_fail_zerosize)
{
    StateMachine sm;

    EXPECT_FALSE(sm.SetStates(testStates, 0));
}

//---------------------------------------------------------------------------
TEST(ut_getcontext_pass)
{
    StateMachine sm;

    EXPECT_TRUE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
    sm.SetContext(nullptr);
    EXPECT_EQUALS(nullptr, sm.GetContext());

    sm.SetContext(&sm);
    EXPECT_EQUALS(&sm, sm.GetContext());
}

//---------------------------------------------------------------------------
TEST(ut_begin_pass)
{
    StateMachine sm;

    EXPECT_TRUE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
    EXPECT_TRUE(sm.Begin());
}

//---------------------------------------------------------------------------
TEST(ut_begin_fail_nostates)
{
    StateMachine sm;

    EXPECT_FALSE(sm.Begin());

    EXPECT_FALSE(sm.SetStates(nullptr, 0));
    EXPECT_FALSE(sm.Begin());
}

//---------------------------------------------------------------------------
TEST(ut_state_jump_pass)
{
    StateMachine sm;

    EXPECT_TRUE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
    EXPECT_TRUE(sm.Begin());

    EXPECT_EQUALS(0, sm.GetCurrentState());

    // Test explicitly jumping from one state to another, using event codes
    // specific to the test state machines

    TestEvent_t event;
    event.eEventCode = TestEventCode::jump_to_c;
    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(2, sm.GetCurrentState());

    event.eEventCode = TestEventCode::jump_to_d;
    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(3, sm.GetCurrentState());

    event.eEventCode = TestEventCode::jump_to_b;
    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(1, sm.GetCurrentState());

    event.eEventCode = TestEventCode::jump_to_e;
    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(4, sm.GetCurrentState());

    event.eEventCode = TestEventCode::jump_to_a;
    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(0, sm.GetCurrentState());

    // Go from one state to the next state in order, using an event
    // code specific to the test state machine

    event.eEventCode = TestEventCode::next_state;

    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(1, sm.GetCurrentState());

    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(2, sm.GetCurrentState());

    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(3, sm.GetCurrentState());

    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(4, sm.GetCurrentState());

    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(0, sm.GetCurrentState());

    // Go from one state to the previous state in order, using an event
    // code specific to the test state machine

    event.eEventCode = TestEventCode::prev_state;

    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(4, sm.GetCurrentState());

    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(3, sm.GetCurrentState());

    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(2, sm.GetCurrentState());

    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(1, sm.GetCurrentState());

    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(0, sm.GetCurrentState());

    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(4, sm.GetCurrentState());
}

//---------------------------------------------------------------------------
TEST(ut_state_pushpop_pass)
{
    StateMachine sm;

    EXPECT_TRUE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
    EXPECT_TRUE(sm.Begin());

    EXPECT_EQUALS(0, sm.GetCurrentState());
    EXPECT_EQUALS(1, sm.GetStackDepth());

    TestEvent_t event;
    event.eEventCode = TestEventCode::push_to_a;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
    EXPECT_EQUALS(2, sm.GetStackDepth());
    EXPECT_EQUALS(0, sm.GetCurrentState());

    event.eEventCode = TestEventCode::pop;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
    EXPECT_EQUALS(1, sm.GetStackDepth());
    EXPECT_EQUALS(0, sm.GetCurrentState());

    event.eEventCode = TestEventCode::push_to_b;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
    EXPECT_EQUALS(2, sm.GetStackDepth());
    EXPECT_EQUALS(1, sm.GetCurrentState());

    event.eEventCode = TestEventCode::pop;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
    EXPECT_EQUALS(1, sm.GetStackDepth());
    EXPECT_EQUALS(0, sm.GetCurrentState());

    event.eEventCode = TestEventCode::push_to_c;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
    EXPECT_EQUALS(2, sm.GetStackDepth());
    EXPECT_EQUALS(2, sm.GetCurrentState());

    event.eEventCode = TestEventCode::pop;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
    EXPECT_EQUALS(1, sm.GetStackDepth());
    EXPECT_EQUALS(0, sm.GetCurrentState());

    event.eEventCode = TestEventCode::push_to_d;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
    EXPECT_EQUALS(2, sm.GetStackDepth());
    EXPECT_EQUALS(3, sm.GetCurrentState());

    event.eEventCode = TestEventCode::pop;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
    EXPECT_EQUALS(1, sm.GetStackDepth());
    EXPECT_EQUALS(0, sm.GetCurrentState());

    event.eEventCode = TestEventCode::push_to_e;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
    EXPECT_EQUALS(2, sm.GetStackDepth());
    EXPECT_EQUALS(4, sm.GetCurrentState());

    event.eEventCode = TestEventCode::pop;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
    EXPECT_EQUALS(1, sm.GetStackDepth());
    EXPECT_EQUALS(0, sm.GetCurrentState());
}

//---------------------------------------------------------------------------
TEST(ut_state_nested_event)
{
    StateMachine sm;

    EXPECT_TRUE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
    EXPECT_TRUE(sm.Begin());

    EXPECT_EQUALS(0, sm.GetCurrentState());
    EXPECT_EQUALS(1, sm.GetStackDepth());

    TestEvent_t event;

    // Event "a" only handled in state "a" - handled OK
    event.eEventCode = TestEventCode::handle_in_a;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));

    // Event "b" only handled in state "b" - will report unhandled.
    event.eEventCode = TestEventCode::handle_in_b;
    EXPECT_EQUALS(StateReturn::unhandled, sm.HandleEvent(&event));

    // Push into state "b" - "b" is the top-level state, "a" is the bottom level.
    // Events not handled in "b" will be passed to "a" for processing.
    // Events not handled in either states "a" or "b" will not be handled at all.
    event.eEventCode = TestEventCode::push_to_b;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
    EXPECT_EQUALS(2, sm.GetStackDepth());
    EXPECT_EQUALS(1, sm.GetCurrentState());

    event.eEventCode = TestEventCode::handle_in_b;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));

    event.eEventCode = TestEventCode::handle_in_a;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));

    event.eEventCode = TestEventCode::handle_in_c;
    EXPECT_EQUALS(StateReturn::unhandled, sm.HandleEvent(&event));

    // Push into state "c", verify that events targeting "a", "b" and "c"
    // all handled
    event.eEventCode = TestEventCode::push_to_c;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));

    EXPECT_EQUALS(3, sm.GetStackDepth());
    EXPECT_EQUALS(2, sm.GetCurrentState());

    event.eEventCode = TestEventCode::handle_in_c;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));

    event.eEventCode = TestEventCode::handle_in_b;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));

    event.eEventCode = TestEventCode::handle_in_a;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
}

//---------------------------------------------------------------------------
TEST(ut_state_nested_jump)
{
    StateMachine sm;

    EXPECT_TRUE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
    EXPECT_TRUE(sm.Begin());

    EXPECT_EQUALS(0, sm.GetCurrentState());
    EXPECT_EQUALS(1, sm.GetStackDepth());

    // Build up a state-stack with states a->b->c->d->e, e on top, a on bottom
    TestEvent_t event;
    event.eEventCode = TestEventCode::push_to_b;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));

    event.eEventCode = TestEventCode::push_to_c;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));

    event.eEventCode = TestEventCode::push_to_d;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));

    event.eEventCode = TestEventCode::push_to_e;
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&event));
    EXPECT_EQUALS(5, sm.GetStackDepth());
    EXPECT_EQUALS(4, sm.GetCurrentState());

    // Handle an event telling us to jump to state "b" from "a" --
    // this is only handled in state "a", and will cause the state machine to
    // "pop"/exit out of e, d, c, and b.  the event will be handled in "a",
    // and then transition to state "b".
    event.eEventCode = TestEventCode::nested_jump;
    event.eJumpSource = TestStateIndex::a;
    event.eJumpDest = TestStateIndex::b;

    EXPECT_EQUALS(StateReturn::transition, sm.HandleEvent(&event));
    EXPECT_EQUALS(1, sm.GetStackDepth());
    EXPECT_EQUALS(1, sm.GetCurrentState());
}

//---------------------------------------------------------------------------
TEST(ut_state_ambiguous_jump)
{
    StateMachine sm;

    static bool ambiguousCalled = false;

    auto errorHandler = [](StateMachine* sm, const StateErrorData_t* err) {
        if (err->eType == StateErrorType::ambiguous_operation) {
            ambiguousCalled = true;
        }
    };

    EXPECT_TRUE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
    EXPECT_TRUE(sm.Begin());
    sm.SetErrorHandler(errorHandler);

    EXPECT_EQUALS(0, sm.GetCurrentState());
    EXPECT_EQUALS(1, sm.GetStackDepth());

    // Build up a state-stack with states a->b->c->d->e, e on top, a on bottom
    TestEvent_t event;
    event.eEventCode = TestEventCode::ambiguous;
    sm.HandleEvent(&event);

    EXPECT_EQUALS(true, ambiguousCalled);
}

//---------------------------------------------------------------------------
TEST(ut_state_stack_overflow)
{
    StateMachine sm;

    static bool overflowCalled = false;

    auto errorHandler = [](StateMachine* sm, const StateErrorData_t* err) {
        if (err->eType == StateErrorType::state_stack_overflow) {
            overflowCalled = true;
        }
    };

    EXPECT_TRUE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
    EXPECT_TRUE(sm.Begin());
    sm.SetErrorHandler(errorHandler);

    // Force a stack overflow - verify that overflow called only once the
    // stack is exhausted.
    TestEvent_t stEvent;
    stEvent.eEventCode = TestEventCode::push_to_a;

    for (int i = 0; i < MAX_STATE_STACK_DEPTH - 1; i++) {
        EXPECT_EQUALS(1 + i, sm.GetStackDepth());
        EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&stEvent));
        EXPECT_FALSE(overflowCalled);
    }
    EXPECT_EQUALS(StateReturn::ok, sm.HandleEvent(&stEvent));
    EXPECT_EQUALS(MAX_STATE_STACK_DEPTH, sm.GetStackDepth());
    EXPECT_TRUE(overflowCalled);
}

//---------------------------------------------------------------------------
TEST(ut_state_underflow)
{
    StateMachine sm;

    static bool underflowCalled = false;

    auto errorHandler = [](StateMachine* sm, const StateErrorData_t* err) {
        if (err->eType == StateErrorType::state_stack_underflow) {
            underflowCalled = true;
        }
    };

    EXPECT_TRUE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
    EXPECT_TRUE(sm.Begin());
    sm.SetErrorHandler(errorHandler);

    EXPECT_EQUALS(0, sm.GetCurrentState());
    EXPECT_EQUALS(1, sm.GetStackDepth());

    // Force a state underflow by popping the only state off the stack
    TestEvent_t event;
    event.eEventCode = TestEventCode::pop;
    EXPECT_FALSE(underflowCalled);
    sm.HandleEvent(&event);
    EXPECT_TRUE(underflowCalled);
}

//---------------------------------------------------------------------------
TEST(ut_state_invalid_transition)
{
    StateMachine sm;

    static bool invalidCalled = false;

    auto errorHandler = [](StateMachine* sm, const StateErrorData_t* err) {
        if (err->eType == StateErrorType::invalid_state) {
            invalidCalled = true;
        }
    };

    EXPECT_TRUE(sm.SetStates(testStates, sizeof(testStates)/sizeof(State_t)));
    EXPECT_TRUE(sm.Begin());
    sm.SetErrorHandler(errorHandler);

    EXPECT_EQUALS(0, sm.GetCurrentState());
    EXPECT_EQUALS(1, sm.GetStackDepth());

    // Force a transition to a state that doesn't exist.  Verify it results
    // in the "invalid state transition" error callback being called.
    TestEvent_t event;
    event.eEventCode = TestEventCode::nested_jump;
    event.eJumpSource = TestStateIndex::a;
    event.eJumpDest = (TestStateIndex)6;

    EXPECT_FALSE(invalidCalled);
    sm.HandleEvent(&event);
    EXPECT_TRUE(invalidCalled);
}

//---------------------------------------------------------------------------
//===========================================================================
// Test Whitelist Goes Here
//===========================================================================
TEST_CASE_START
TEST_CASE(ut_setstates_pass),
TEST_CASE(ut_setstates_fail_2Xinit),
TEST_CASE(ut_setstates_fail_nullstates),
TEST_CASE(ut_setstates_fail_zerosize),
TEST_CASE(ut_getcontext_pass),
TEST_CASE(ut_begin_pass),
TEST_CASE(ut_begin_fail_nostates),
TEST_CASE(ut_state_jump_pass),
TEST_CASE(ut_state_pushpop_pass),
TEST_CASE(ut_state_nested_event),
TEST_CASE(ut_state_nested_jump),
TEST_CASE(ut_state_ambiguous_jump),
TEST_CASE(ut_state_stack_overflow),
TEST_CASE(ut_state_underflow),
TEST_CASE(ut_state_invalid_transition),
TEST_CASE_END
} // namespace Mark3
