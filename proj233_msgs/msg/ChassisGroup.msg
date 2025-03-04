#底盘响应加速度；单位：m/s^2;取值：底盘控制器实际反馈值；
#约定特殊值10000.f表示状态异常或状态未知；
float32    acceleration_acknowledge

#当前挡位所在位置；取值：0-未知；1-P；2-R；3-N；4-D；5-M；6-S；7-L；8~10:预留；      11~20-D1~D10(即1档到10档(若有))；
uint8      gear_position

#油门踏板开度; 单位：1%； 取值：底盘控制器实际反馈值；合理范围：0~100；
#约定特殊值255U表示状态异常或状态未知；
uint8      acceleration_pedal_ratio

#驾驶员踩下制动踏板的状态；取值：0-未踩下; 1-踩下; 其他值表示状态异常或未知； 
uint8      driver_brake_pedal_pressed

#主缸压力; 单位：Mpa; 取值：底盘控制器实际反馈值；
#约定特殊值10000.f表示状态异常或状态未知；
float32    master_cylinder_pressure

#驻车/刹车 卡钳状态; 取值：0-已释放; 1-已抱闸; 2-释放中; 3-抱闸中；4-未知；
uint8      brake_status

#ABS激活状态; 取值：true-激活；false-未激活；
bool       abs_active

#方向盘转角速率; 单位：度/秒(Degree/s); 逆时针为正；取值：底盘控制器实际反馈值；
#约定特殊值10000.f表示状态异常或状态未知；
float32    steer_wheel_angle_rate

#驾驶员Hands-off的置信度; 单位：1%；取值：底盘控制器实际反馈值；
#约定特殊值255U表示状态异常或状态未知；
uint8      hands_off_conf

#方向盘转向扭矩; 单位：Nm; 逆时针为正；取值：底盘控制器实际反馈值；
#约定特殊值10000.f表示状态异常或状态未知；
float32    steer_torque