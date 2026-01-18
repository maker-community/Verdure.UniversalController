// 中文字库扩展 - 贪吃蛇游戏专用
// 需要添加的汉字：历 史 记 录 总 局 开 始 退 出 暂 停

/*
 * 说明：
 * 这是一个补充字库框架，需要使用工具生成实际数据
 * 
 * 使用方法：
 * 1. 使用 lcd-image-converter 或在线工具生成这些汉字的字库数据
 * 2. 将生成的数组数据填充到下方
 * 3. 在 SnakeGame.cpp 中加载此字库
 * 
 * 工具推荐：
 * - lcd-image-converter: https://github.com/riuson/lcd-image-converter
 * - LVGL在线工具: https://lvgl.100ask.net/
 */

#ifndef CHINESE_32_EXT_H
#define CHINESE_32_EXT_H

#include <pgmspace.h>

// TODO: 这里需要填充工具生成的字库数据
// 格式参考 chinese_32.h

// 临时方案：直接返回使用英文替代
// 等工具生成后再替换

/*
const uint8_t chinese_32_ext[] PROGMEM = {
    // 将工具生成的数据粘贴在这里
    // 应该包含：历 史 记 录 总 局 开 始 退 出 暂 停
};
*/

#endif
