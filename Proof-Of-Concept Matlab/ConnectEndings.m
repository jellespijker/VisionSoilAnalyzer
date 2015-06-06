function [ O ] = ConnectEndings( I )
%CONNECTENDINGS ( I ) Connect the broken edge creating a continuing fucntion
%   Copyright (c) 2014 Jelle Spijker
% 
%  Permission is hereby granted, free of charge, to any person
%  obtaining a copy of this software and associated documentation
%  files (the "Software"), to deal in the Software without
%  restriction, including without limitation the rights to use,
%  copy, modify, merge, publish, distribute, sublicense, and/or sell
%  copies of the Software, and to permit persons to whom the
%  Software is furnished to do so, subject to the following
%  conditions:
% 
%  The above copyright notice and this permission notice shall be
%  included in all copies or substantial portions of the Software.
% 
%  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
%  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
%  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
%  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
%  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
%  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
%  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
%  OTHER DEALINGS IN THE SOFTWARE.

[X, Y] = size(I);

[~, ~, re, ce] = findendsjunctions(I);

if size(re) == [0 0]
    O = I;
    return
end

C = [re ce];

[s, ~] = size(C);
if mod(s,2) ~= 0
    error('uneven endings');
end

for i = 1:s
   % Calc relative distance for each point
   D = [(C(:,1)- C(i,1)) (C(:,2) - C(i,2))];
   D = D.^2;
   dist = D(:,1) + D(:,2);
   %dist(dist) = nan;
   
   % Build connection matrix
   Con = [C(i,:); C(dist == min(dist(dist > 0)),:)];
   
   % fill the gasp
   nsteps = Con(2,1) - Con(1,1);
   msteps = Con(2,2) - Con(1,2);
   
   if nsteps ~= 0 && msteps ~= 0
       I(Con(1,1)+1:Con(2,1)-1, Con(1,2)+1:Con(2,2)-1) = 1;
   elseif nsteps == 0
       I(Con(1,1):Con(2,1), Con(1,2)+1:Con(2,2)-1) = 1;
   elseif msteps == 0
       I(Con(1,1)+1:Con(2,1)-1, Con(1,2)+1:Con(2,2)-1) = 1;
   end
    
end

O = I;

end

