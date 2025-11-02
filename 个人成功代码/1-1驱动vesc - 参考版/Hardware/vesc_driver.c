#include "vesc_driver.h"
#include "string.h"

// 静态函数声明
static void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t* index);
static void CAN_SendExtendedData(uint32_t extId, uint8_t* data, uint8_t len);

/**
 * @brief 初始化VESC驱动
 */
void VESC_Init(void)
{
    // CAN已经在主函数中初始化，这里可以添加VESC特定的初始化代码
}

/**
 * @brief 设置VESC转速
 * @param vesc_id: VESC设备ID (0-255)
 * @param rpm: 转速值 (电气RPM)
 */
void VESC_SetRPM(uint8_t vesc_id, int32_t rpm)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    
    // 将转速值打包到缓冲区
    buffer_append_int32(buffer, rpm, &send_index);
    
    // 构建CAN ID并发送
    uint32_t can_id = ((uint32_t)CAN_PACKET_SET_RPM << 8) | vesc_id;
    CAN_SendExtendedData(can_id, buffer, send_index);
	
}

/**
 * @brief 设置VESC电流
 * @param vesc_id: VESC设备ID (0-255)
 * @param current: 电流值 (安培)
 */
void VESC_SetCurrent(uint8_t vesc_id, float current)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    
    // 电流放大1000倍转换为整数
    buffer_append_int32(buffer, (int32_t)(current * 1000.0f), &send_index);
    
    uint32_t can_id = ((uint32_t)CAN_PACKET_SET_CURRENT << 8) | vesc_id;
    CAN_SendExtendedData(can_id, buffer, send_index);
}

/**
 * @brief 设置VESC占空比
 * @param vesc_id: VESC设备ID (0-255)
 * @param duty: 占空比 (-1.0 到 1.0)
 */
void VESC_SetDutyCycle(uint8_t vesc_id, float duty)
{
    int32_t send_index = 0;
    uint8_t buffer[4];
    
    // 占空比放大100000倍转换为整数
    buffer_append_int32(buffer, (int32_t)(duty * 100000.0f), &send_index);
    
    uint32_t can_id = ((uint32_t)CAN_PACKET_SET_DUTY << 8) | vesc_id;
	//CAN_PACKET_SET_DUTY   这个是设置模式的
	
    CAN_SendExtendedData(can_id, buffer, send_index);
}

/**
 * @brief 将32位整数以大端序添加到缓冲区
 * @param buffer: 目标缓冲区
 * @param number: 要添加的整数
 * @param index: 当前缓冲区索引指针
 */
static void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t* index)
{
    buffer[(*index)++] = (number >> 24) & 0xFF;
    buffer[(*index)++] = (number >> 16) & 0xFF;
    buffer[(*index)++] = (number >> 8) & 0xFF;
    buffer[(*index)++] = number & 0xFF;
}

/**
 * @brief 发送扩展ID的CAN数据帧
 * @param extId: 扩展ID
 * @param data: 数据指针
 * @param len: 数据长度 (0-8)
 */
static void CAN_SendExtendedData(uint32_t extId, uint8_t* data, uint8_t len)
{
    CanTxMsg TxMessage;
    
    // 清除结构体
    memset(&TxMessage, 0, sizeof(TxMessage));
    
    // 配置发送消息
    TxMessage.ExtId = extId;           // 扩展ID
    TxMessage.IDE = CAN_Id_Extended;   // 使用扩展ID
    TxMessage.RTR = CAN_RTR_Data;      // 数据帧
    TxMessage.DLC = len;               // 数据长度
    
    // 拷贝数据
    for(uint8_t i = 0; i < len; i++) {
        TxMessage.Data[i] = data[i];
    }
    
    // 发送消息
    CAN_Transmit(CAN1, &TxMessage);
    
    // while(CAN_TransmitStatus(CAN1, CAN_FIFO0) == CANTXFailed);
}

