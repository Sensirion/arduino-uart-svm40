/*
 * THIS FILE IS AUTOMATICALLY GENERATED AND MUST NOT BE EDITED MANUALLY!
 *
 * SHDLC-Generator: 0.8.2
 * Yaml Version: 0.3.0
 * Template Version: 0.3.0
 */
/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "SensirionUartSvm40.h"
#include "Arduino.h"
#include "SensirionCore.h"
#include <math.h>

#define SVM40_UART_ADDRESS 0x00

SensirionUartSvm40::SensirionUartSvm40() {
}

void SensirionUartSvm40::begin(Stream& serial) {
    _serial = &serial;
}

uint16_t SensirionUartSvm40::startContinuousMeasurement() {
    uint16_t error;
    uint8_t buffer[12];
    SensirionShdlcTxFrame txFrame(buffer, 12);
    SensirionShdlcRxFrame rxFrame(buffer, 12);

    error = txFrame.begin(0x00, SVM40_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x00);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);

    return error;
}

uint16_t SensirionUartSvm40::stopMeasurement() {
    uint16_t error;
    uint8_t buffer[12];
    SensirionShdlcTxFrame txFrame(buffer, 12);
    SensirionShdlcRxFrame rxFrame(buffer, 12);

    error = txFrame.begin(0x01, SVM40_UART_ADDRESS, 0);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);

    return error;
}

uint16_t SensirionUartSvm40::readMeasuredValuesAsIntegers(
    int16_t& vocIndex, int16_t& humidity, int16_t& temperature) {
    uint16_t error;
    uint8_t buffer[24];
    SensirionShdlcTxFrame txFrame(buffer, 24);
    SensirionShdlcRxFrame rxFrame(buffer, 24);

    error = txFrame.begin(0x03, SVM40_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x0A);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);
    if (error) {
        return error;
    }

    error |= rxFrame.getInt16(vocIndex);
    error |= rxFrame.getInt16(humidity);
    error |= rxFrame.getInt16(temperature);
    return error;
}

uint16_t SensirionUartSvm40::readMeasuredValuesAsIntegersWithRawParameters(
    int16_t& vocIndex, int16_t& humidity, int16_t& temperature,
    uint16_t& rawVocTicks, int16_t& rawHumidity, int16_t& rawTemperature) {
    uint16_t error;
    uint8_t buffer[36];
    SensirionShdlcTxFrame txFrame(buffer, 36);
    SensirionShdlcRxFrame rxFrame(buffer, 36);

    error = txFrame.begin(0x03, SVM40_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x0B);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);
    if (error) {
        return error;
    }

    error |= rxFrame.getInt16(vocIndex);
    error |= rxFrame.getInt16(humidity);
    error |= rxFrame.getInt16(temperature);
    error |= rxFrame.getUInt16(rawVocTicks);
    error |= rxFrame.getInt16(rawHumidity);
    error |= rxFrame.getInt16(rawTemperature);
    return error;
}

uint16_t SensirionUartSvm40::readMeasuredValues(float& vocIndex,
                                                float& humidity,
                                                float& temperature) {
    uint16_t error;
    int16_t humidityRaw;
    int16_t temperatureRaw;
    int16_t vocRaw;

    error = readMeasuredValuesAsIntegers(vocRaw, humidityRaw, temperatureRaw);
    if (error) {
        return error;
    }

    humidity = static_cast<float>(humidityRaw) / 100.0f;
    temperature = static_cast<float>(temperatureRaw) / 200.0f;
    vocIndex = static_cast<float>(vocRaw) / 10.0f;
    return NoError;
}

uint16_t SensirionUartSvm40::getTemperatureOffsetForRhtMeasurementsRaw(
    int16_t& tOffset) {
    uint16_t error;
    uint8_t buffer[20];
    SensirionShdlcTxFrame txFrame(buffer, sizeof buffer);
    SensirionShdlcRxFrame rxFrame(buffer, sizeof buffer);

    error = txFrame.begin(0x60, SVM40_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x01);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);
    if (error) {
        return error;
    }

    error |= rxFrame.getInt16(tOffset);
    return error;
}

uint16_t
SensirionUartSvm40::getTemperatureOffsetForRhtMeasurements(float& tOffset) {
    uint16_t error;
    int16_t tOffsetRaw;

    tOffset = NAN;

    error = getTemperatureOffsetForRhtMeasurementsRaw(tOffsetRaw);

    if (error) {
        return error;
    }

    tOffset = static_cast<float>(tOffsetRaw) / 200.0f;
    return NoError;
}

uint16_t SensirionUartSvm40::getVocTuningParameters(
    int16_t& vocIndexOffset, int16_t& learningTimeHours,
    int16_t& gatingMaxDurationMinutes, int16_t& stdInitial) {
    uint16_t error;
    uint8_t buffer[28];
    SensirionShdlcTxFrame txFrame(buffer, 28);
    SensirionShdlcRxFrame rxFrame(buffer, 28);

    error = txFrame.begin(0x60, SVM40_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x08);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);
    if (error) {
        return error;
    }

    error |= rxFrame.getInt16(vocIndexOffset);
    error |= rxFrame.getInt16(learningTimeHours);
    error |= rxFrame.getInt16(gatingMaxDurationMinutes);
    error |= rxFrame.getInt16(stdInitial);
    return error;
}

uint16_t SensirionUartSvm40::storeNvData() {
    uint16_t error;
    uint8_t buffer[12];
    SensirionShdlcTxFrame txFrame(buffer, 12);
    SensirionShdlcRxFrame rxFrame(buffer, 12);

    error = txFrame.begin(0x60, SVM40_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x80);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 500000);

    return error;
}

uint16_t
SensirionUartSvm40::setTemperatureOffsetForRhtMeasurementsRaw(int16_t tOffset) {
    uint16_t error;
    uint8_t buffer[16];
    SensirionShdlcTxFrame txFrame(buffer, sizeof buffer);
    SensirionShdlcRxFrame rxFrame(buffer, sizeof buffer);

    error = txFrame.begin(0x60, SVM40_UART_ADDRESS, 3);
    error |= txFrame.addUInt8(0x81);
    error |= txFrame.addInt16(tOffset);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);

    return error;
}

uint16_t
SensirionUartSvm40::setTemperatureOffsetForRhtMeasurements(float tOffset) {
    int16_t tOffsetRaw = static_cast<int16_t>(tOffset * 200.0f);
    return setTemperatureOffsetForRhtMeasurementsRaw(tOffsetRaw);
}

uint16_t SensirionUartSvm40::setVocTuningParameters(
    int16_t vocIndexOffset, int16_t learningTimeHours,
    int16_t gatingMaxDurationMinutes, int16_t stdInitial) {
    uint16_t error;
    uint8_t buffer[28];
    SensirionShdlcTxFrame txFrame(buffer, 28);
    SensirionShdlcRxFrame rxFrame(buffer, 28);

    error = txFrame.begin(0x60, SVM40_UART_ADDRESS, 9);
    error |= txFrame.addUInt8(0x88);
    error |= txFrame.addInt16(vocIndexOffset);
    error |= txFrame.addInt16(learningTimeHours);
    error |= txFrame.addInt16(gatingMaxDurationMinutes);
    error |= txFrame.addInt16(stdInitial);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);

    return error;
}

uint16_t SensirionUartSvm40::getVocState(uint8_t state[], uint8_t stateSize) {
    uint16_t error;
    uint8_t buffer[28];
    SensirionShdlcTxFrame txFrame(buffer, 28);
    SensirionShdlcRxFrame rxFrame(buffer, 28);

    error = txFrame.begin(0x61, SVM40_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x08);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);
    if (error) {
        return error;
    }

    error |= rxFrame.getBytes(state, stateSize);
    return error;
}

uint16_t SensirionUartSvm40::setVocState(uint8_t state[], uint8_t stateSize) {
    uint16_t error;
    uint8_t buffer[28];
    SensirionShdlcTxFrame txFrame(buffer, 28);
    SensirionShdlcRxFrame rxFrame(buffer, 28);

    error = txFrame.begin(0x61, SVM40_UART_ADDRESS, 9);
    error |= txFrame.addUInt8(0x88);
    error |= txFrame.addBytes(state, stateSize);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);

    return error;
}

uint16_t SensirionUartSvm40::getProductType(uint8_t productType[],
                                            uint8_t productTypeSize) {
    uint16_t error;
    uint8_t buffer[522];
    SensirionShdlcTxFrame txFrame(buffer, 522);
    SensirionShdlcRxFrame rxFrame(buffer, 522);

    error = txFrame.begin(0xD0, SVM40_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x00);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);
    if (error) {
        return error;
    }

    error |= rxFrame.getBytes(productType, productTypeSize);
    return error;
}

uint16_t SensirionUartSvm40::getProductName(uint8_t productName[],
                                            uint8_t productNameSize) {
    uint16_t error;
    uint8_t buffer[522];
    SensirionShdlcTxFrame txFrame(buffer, 522);
    SensirionShdlcRxFrame rxFrame(buffer, 522);

    error = txFrame.begin(0xD0, SVM40_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x01);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);
    if (error) {
        return error;
    }

    error |= rxFrame.getBytes(productName, productNameSize);
    return error;
}

uint16_t SensirionUartSvm40::getSerialNumber(uint8_t serialNumber[],
                                             uint8_t serialNumberSize) {
    uint16_t error;
    uint8_t buffer[522];
    SensirionShdlcTxFrame txFrame(buffer, 522);
    SensirionShdlcRxFrame rxFrame(buffer, 522);

    error = txFrame.begin(0xD0, SVM40_UART_ADDRESS, 1);
    error |= txFrame.addUInt8(0x03);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);
    if (error) {
        return error;
    }

    error |= rxFrame.getBytes(serialNumber, serialNumberSize);
    return error;
}

uint16_t
SensirionUartSvm40::getVersion(uint8_t& firmwareMajor, uint8_t& firmwareMinor,
                               bool& firmwareDebug, uint8_t& hardwareMajor,
                               uint8_t& hardwareMinor, uint8_t& protocolMajor,
                               uint8_t& protocolMinor) {
    uint16_t error;
    uint8_t buffer[26];
    SensirionShdlcTxFrame txFrame(buffer, 26);
    SensirionShdlcRxFrame rxFrame(buffer, 26);

    error = txFrame.begin(0xD1, SVM40_UART_ADDRESS, 0);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);
    if (error) {
        return error;
    }

    error |= rxFrame.getUInt8(firmwareMajor);
    error |= rxFrame.getUInt8(firmwareMinor);
    error |= rxFrame.getBool(firmwareDebug);
    error |= rxFrame.getUInt8(hardwareMajor);
    error |= rxFrame.getUInt8(hardwareMinor);
    error |= rxFrame.getUInt8(protocolMajor);
    error |= rxFrame.getUInt8(protocolMinor);
    return error;
}

uint16_t SensirionUartSvm40::deviceReset() {
    uint16_t error;
    uint8_t buffer[12];
    SensirionShdlcTxFrame txFrame(buffer, 12);
    SensirionShdlcRxFrame rxFrame(buffer, 12);

    error = txFrame.begin(0xD3, SVM40_UART_ADDRESS, 0);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);

    delay(100);
    return error;
}

uint16_t SensirionUartSvm40::getSystemUpTime(uint32_t& systemUpTime) {
    uint16_t error;
    uint8_t buffer[20];
    SensirionShdlcTxFrame txFrame(buffer, 20);
    SensirionShdlcRxFrame rxFrame(buffer, 20);

    error = txFrame.begin(0x93, SVM40_UART_ADDRESS, 0);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);
    if (error) {
        return error;
    }

    error |= rxFrame.getUInt32(systemUpTime);
    return error;
}

uint16_t SensirionUartSvm40::enterBootloader() {
    uint16_t error;
    uint8_t buffer[12];
    SensirionShdlcTxFrame txFrame(buffer, 12);
    SensirionShdlcRxFrame rxFrame(buffer, 12);

    error = txFrame.begin(0xF3, SVM40_UART_ADDRESS, 0);
    error |= txFrame.addUInt8(0x00);
    error |= txFrame.finish();
    if (error) {
        return error;
    }

    error = SensirionShdlcCommunication::sendAndReceiveFrame(*_serial, txFrame,
                                                             rxFrame, 50000);

    delay(1000);
    return error;
}
