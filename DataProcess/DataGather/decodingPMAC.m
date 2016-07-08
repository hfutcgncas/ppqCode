function rt = decodingPMAC(strData,type)
if(strcmp(type , 'double'))
    MANTISA = strData(1:8);
    LOW_VALUE = strData(1,9);
    EXP = strData(10:12);
    value_hi = hex2dec(MANTISA);
    value_low = hex2dec(LOW_VALUE);
    exponent = hex2dec(EXP) - 2047;
    value = ( (16 * value_hi) +value_low) /34359738368.0;
    rt = value * 2^ exponent;
    return;
end
if(strcmp(type , 'int'))
    Low_word = strData(7:12);
    High_word = strData(1:6);
    value_low = hex2dec(Low_word);
    value_high = hex2dec(High_word);
    if(value_high > 8388608)
        value_high = value_high - 16777216 ;
    end
    rt = 16777216.0 * value_high +value_low;
    return;
end
if(strcmp(type , 'single'))
    value_low = hex2dec(strData);
    if(value_low >= 838608)
        value_low = value_low - 16777216;
    end
    rt = value_low;
end
end