function [ s ] = ifrdescp( z, nd )
%IFRDESCP Computes inverse Fourier descriptors
%   Source: Gonzalez. Digital Image Processing Using MATLAB. McGraw-Hill Education (India) Pvt Limited, 2013.
% Some changes made by Jelle Spijker

% Preliminaries.
np = length(z);

% Check inputs.
if nargin == 1
    nd = np;
end
if mod(np,2) ~= 0
    error('length(z) must be an even integer.')
elseif mod(nd,2) ~= 0
    error('nd must be an even integer.')
end
% Create an alternating sequence of 1s and -1s for us in centering the
% transform
x = 0:(np - 1);
m = ((-1) .^ x)';

%use only nd descriptors in the inverse. Because the descriptors are
%centered, (np - nd)/2 terms from each end of the sequence are set to 0.

d = (np - nd)/2;
z(1:d) = 0;
z(np - d + 1:np) = 0;

% Compute the inverse and convert back to coordinates.
zz = ifft(z);
s(:,1) = real(zz);
s(:,2) = imag(zz);

% Multiply by alternating 1 and -s to undo the earlier centering.
s(:,1) = round(m .* s(:,1));
s(:,2) = round(m .* s(:,2));

s = uint16(s);
end

