// 
// 
// 

#include "MotorManager.h"

ESP::MotorManager::MotorManager() : m_motor(RoboApi::MotorController(MOTOR_L_IA, MOTOR_L_IB), RoboApi::MotorController(MOTOR_R_IA, MOTOR_R_IB))
{
    CmdCallback callback;
    callback.cmdHandler = (cmdHandler_fn)handlerMove;
    callback.param = this;
    callback.triggerMask = FL_MOVE;
    callback.bMaskNot = false;
    CmdManager::instance()->registerCmdCallback(callback);
    callback.cmdHandler = (cmdHandler_fn)handlerStop;
    callback.bMaskNot = true;
}

void ESP::MotorManager::handlerMove(EspCmd cmd, MotorManager* _this)
{
    ESP::EspCmd::move_t move = cmd.move;
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

void ESP::MotorManager::handlerStop(EspCmd cmd, MotorManager* _this)
{
    _this->m_motor.stop();
}

ESP::MotorManager* ESP::MotorManager::instance()
{
    static MotorManager g_instance;
    return &g_instance;
}

void ESP::MotorManager::createMove(int forward, int left, bool bStop)
{
    EspCmd cmd;
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
    CmdManager::instance()->sendCmd(cmd);
}
