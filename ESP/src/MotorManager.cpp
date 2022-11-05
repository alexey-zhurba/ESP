// 
// 
// 

#include "MotorManager.h"

_ESP::MotorManager::MotorManager() : m_motor(RoboApi::MotorController(MOTOR_L_IA, MOTOR_L_IB), RoboApi::MotorController(MOTOR_R_IA, MOTOR_R_IB))
{
    CmdCallback callback;
    callback.cmdHandler = (cmdHandler_fn)handlerMove;
    callback.param = this;
    callback.triggerMask = FL_MOVE;
    callback.bMaskNot = false;
    CmdManager::instance()->registerCmdCallback(callback);
    callback.cmdHandler = (cmdHandler_fn)handlerStop;
    callback.bMaskNot = true;
    CmdManager::instance()->registerCmdCallback(callback);
}

void _ESP::MotorManager::handlerMove(EspCmd cmd, MotorManager* _this)
{
    _ESP::EspCmd::move_t move = cmd.move;
    if (!move.forward && !move.left
        || move.forward && move.left //kein simultanes drehen und fahren -> kein joystick
        )
    {
        //TODO: error "illegal command"
        return;
    }
    if(move.left > 0)
    {
        _this->m_motor.links(move.left);
    }
    else if (move.left < 0)
    {
        _this->m_motor.rechts(-move.left);
    }
    if (move.forward > 0)
    {
        _this->m_motor.vorwaerts(move.forward);
    }
    else if (move.forward < 0)
    {
        _this->m_motor.rueckwaerts(-move.forward);
    }
}

void _ESP::MotorManager::handlerStop(EspCmd cmd, MotorManager* _this)
{
    _this->m_motor.stop();
}

_ESP::MotorManager* _ESP::MotorManager::instance()
{
    static MotorManager g_instance;
    return &g_instance;
}

void _ESP::MotorManager::createMove(int forward, int left, bool bStop, CmdOrigin origin)
{
    EspCmd cmd;
    PRINT_ENTER_FUNC();
    cmd.origin = origin;
    if (!bStop) 
    {
        cmd.flags = CmdManager::instance()->stateFlags() | FL_MOVE;
        cmd.move.forward = forward;
        cmd.move.left = left;
    }
    else 
    {
        cmd.flags = CmdManager::instance()->stateFlags() & (~FL_MOVE);
    }
    
    DEBUG_PRINT("creating move cmd. flags: ");
    DEBUG_PRINTLN(cmd.flags);
    CmdManager::instance()->sendCmd(cmd);
    PRINT_EXIT_FUNC();
}
