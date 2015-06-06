clear;
C = cell(83+115+142+106, 5);
T = zeros(83+115+142+106, 1);

counter = 1;
for i = 1:4
   load(sprintf('matlab%d.mat', i));
   [~, s] = size(DB);
   for j = 1:s
       C{counter, 1} = DB(j).z;
       C{counter, 2} = ifrdescp(C{counter,1},6);
       C{counter, 3} = bound2im(C{counter,2});
       if (size(C{counter,1}) >= [6 1])
            C{counter, 4} = C{counter,1}(1:6);
       end
       s = regionprops(C{counter, 3}, 'Eccentricity');
       C{counter, 5} = s.Eccentricity;
       T(counter) = s.Eccentricity;
       counter = counter + 1;
   end
end

C(193,:) = [];

I = zeros(445,12);
O = zeros(445,1);
for i = 1:445
    I(i,:) = [phase(C{i,4}); angle(C{i,4})];
    O(i) = C{i,5};
end
