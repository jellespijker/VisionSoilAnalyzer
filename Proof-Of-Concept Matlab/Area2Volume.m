function [ V ] = Area2Volume( A )
%Calculate the theoretical volume of a given Area
%   See Literatuur studie
    V = power(A,(3/2))/(6*sqrt(pi));
end

