/*
 * Copyright (c) 2018-2021 Ampere Computing LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <ipmid/api-types.hpp>
#include <user_channel/user_layer.hpp>

constexpr uint8_t responseFail = 0x01;

namespace ipmi
{
namespace rick
{
constexpr uint8_t netFnAPP = 0x30;
} // namespace rick
namespace app
{
constexpr Cmd cmdGetTemp275 = 0x01;
constexpr Cmd cmdSetThreshold = 0x02;
} // namespace app
} // namespace ipmi
