function [ z ] = frdescp( particle )
%FRDESCP Compute Fourier descriptors
%   Source: Gonzalez. Digital Image Processing Using MATLAB. McGraw-Hill Education (India) Pvt Limited, 2013.

b = bwboundaries(particle, 'noholes');
s = b{1};

[np, nc] = size(s);
if nc ~= 2
    error('S must be of size np-by-2.');
end
if np/2 ~= round(np/2);
    s(end + 1, :) = s(end, :);
    np = np + 1;
end

% Create an alternating sequence of 1s and -1s for use in centering the
% transform.
x = 0:(np -1);
m = ((-1) .^x)';

% Multiply the input sequence by alternating 1s and -1s to center the
% transform
s(:, 1) = m .* s(:, 1);
s(:, 2) = m .* s(:, 2);

% Convert coordinates to complex numbers.
s = s(:, 1) + i*s(:,2);

% Compute the descriptors
z = fft(s);

% Opschalen????
end

