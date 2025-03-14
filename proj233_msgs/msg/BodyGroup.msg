#道路坡度; 单位：rad; 取值：底盘控制器实际反馈值；约定特殊值10000.f表示状态异常或状态未知；
float32 inclination

#转向拨杆状态; 取值：0-关; 1-左转; 2-右转; 3-未知；
int8    turn_light_lever_status

#转向灯状态; 取值：0-左&右关; 1-左转闪; 2-右转闪; 3-左&右闪；4-未知；
int8    turn_light_status

#远光灯状态; 取值：0-关; 1-开; 2-未知；
int8    high_beam_status

#前雾灯状态; 取值：0-前&后关; 1-前开; 2-后开；3-前&后开；4-未知；
int8    fog_light_status

#近光灯状态; 取值：0-关; 1-开; 2-未知；
int8    low_beam_status