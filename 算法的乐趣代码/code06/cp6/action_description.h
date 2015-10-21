// action_description.h : include file for action description interface
//

#pragma once

#include "river_def.h"

typedef struct tagActionDescription
{
    ACTION_NAME act;
    const char *description; //��Ӧ������������������
}ACTION_DESCRIPTION;


const char *GetActionDescription(ACTION_NAME act);



