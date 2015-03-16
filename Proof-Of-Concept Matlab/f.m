function [ y ] = f( t )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

if t > 0.008856
    y = t^(1/3);
else
    y = 7.787 * t + (16/116);
end
end

