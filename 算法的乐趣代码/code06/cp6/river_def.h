// river_def.h : include file for river project
//

#pragma once

typedef enum tagBoatLocation
{
    LOCAL = 0,
    REMOTE
}BOAT_LOCATION;

typedef enum tagActionName
{
    ONE_MONSTER_GO = 0,
    TWO_MONSTER_GO,
    ONE_MONK_GO,
    TWO_MONK_GO,
    ONE_MONSTER_ONE_MONK_GO,
    ONE_MONSTER_BACK,
    TWO_MONSTER_BACK,
    ONE_MONK_BACK,
    TWO_MONK_BACK,
    ONE_MONSTER_ONE_MONK_BACK,
    INVALID_ACTION_NAME,
}ACTION_NAME;

typedef struct tagActionEffection
{
    ACTION_NAME act;
    BOAT_LOCATION boat_to; //���ƶ��ķ���
    int move_monster; //�˴��ƶ�����������
    int move_monk; //�˴��ƶ��ĺ�������
}ACTION_EFFECTION;



