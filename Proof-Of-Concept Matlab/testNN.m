ok = zeros(445,1);

for i = 1:445
   tt =  myNeuralNetworkFunction(I(i));
   t = find(tt==max(tt));
   if O(i,t) == 1
       ok(i) = 1;
   else
       ok(i) = 0;
   end
end