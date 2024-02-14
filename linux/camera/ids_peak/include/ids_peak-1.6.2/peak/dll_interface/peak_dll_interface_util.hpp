/*!
 * \file    peak_dll_interface_util.hpp
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0
 *
 * Copyright (c) 2019 - 2023, IDS Imaging Development Systems GmbH. All rights reserved.
 */

#pragma once


#include <peak/backend/peak_backend.h>
#include <peak/common/peak_common_structs.hpp>
#include <peak/error_handling/peak_error_handling.hpp>
#include <peak/generic/peak_class_creator.hpp>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>

namespace
{

template <typename Callable>
inline void CallAndCheckCInterfaceFunction(Callable&& cInterfaceFunction)
{
    peak::core::ExecuteAndMapReturnCodes(std::forward<Callable>(cInterfaceFunction));
}

template <typename NumericType, typename Callable>
NumericType QueryNumericFromCInterfaceFunction(const Callable& cInterfaceFunction)
{
    NumericType numeric{};

    peak::core::ExecuteAndMapReturnCodes([&] { return cInterfaceFunction(&numeric); });

    return numeric;
}

template <typename Callable>
inline peak::core::RawInformation QueryRawInformationFromCInterfaceFunction(const Callable& cInterfaceFunction)
{
    int32_t dataType = 0;
    size_t dataSize = 0;

    peak::core::ExecuteAndMapReturnCodes([&] { return cInterfaceFunction(&dataType, nullptr, &dataSize); });
    std::vector<uint8_t> buffer(dataSize);
    peak::core::ExecuteAndMapReturnCodes(
        [&] { return cInterfaceFunction(&dataType, buffer.data(), &dataSize); });

    return peak::core::RawInformation{ dataType, std::move(buffer) };
}

template <typename Callable>
inline std::string QueryStringFromCInterfaceFunction(const Callable& cInterfaceFunction)
{
    auto strSize = QueryNumericFromCInterfaceFunction<size_t>(
        [&](size_t* _strSize) { return cInterfaceFunction(nullptr, _strSize); });

    std::vector<char> str(strSize);
    peak::core::ExecuteAndMapReturnCodes([&] { return cInterfaceFunction(str.data(), &strSize); });

    return std::string(str.data(), strSize - 1);
}

template <typename Callable>
inline std::string QueryStringFromCInterfaceFunctionWithBufferSize(const Callable& cInterfaceFunction, size_t bufferSize = 128)
{
    std::vector<char> str(bufferSize);

    const auto ret = cInterfaceFunction(str.data(), &bufferSize);

    if (ret == PEAK_RETURN_CODE_BUFFER_TOO_SMALL)
    {
        // Note: We have to query the size first with buffer = nullptr, according to the GenTL standard.
        auto strSize = QueryNumericFromCInterfaceFunction<size_t>(
            [&](size_t* _strSize) { return cInterfaceFunction(nullptr, _strSize); });

        str.resize(strSize);
        peak::core::ExecuteAndMapReturnCodes([&] { return cInterfaceFunction(str.data(), &strSize); });
    }
    else if (ret != PEAK_RETURN_CODE_SUCCESS)
    {
        peak::core::ExecuteAndMapReturnCodes([&] { return ret; });
    }

    return {str.data(), bufferSize - 1};
}

template <typename Callable1, typename Callable2>
inline std::vector<std::string> QueryStringArrayFromCInterfaceFunction(
    const Callable1& cInterfaceFunctionArraySize, const Callable2& cInterfaceFunctionArrayMember)
{
    auto arraySize = QueryNumericFromCInterfaceFunction<size_t>(cInterfaceFunctionArraySize);

    std::vector<std::string> array;
    for (size_t x = 0; x < arraySize; ++x)
    {
        array.emplace_back(QueryStringFromCInterfaceFunction(
            [&](char* buffer, size_t* bufferSize) { return cInterfaceFunctionArrayMember(x, buffer, bufferSize); }));
    }

    return array;
}

template <typename NumericType, typename Callable>
std::vector<NumericType> QueryNumericArrayFromCInterfaceFunction(const Callable& cInterfaceFunction)
{
    auto arraySize = QueryNumericFromCInterfaceFunction<size_t>(
        [&](size_t* _arraySize) { return cInterfaceFunction(nullptr, _arraySize); });

    std::vector<NumericType> array(arraySize);
    peak::core::ExecuteAndMapReturnCodes([&] { return cInterfaceFunction(array.data(), &arraySize); });

    return array;
}

} // namespace
