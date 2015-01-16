clear;
C = cell(83+115+142+106, 6);
T = zeros(83+115+142+106, 1);

counter = 1;
for i = 1:4
   load(sprintf('matlab%d.mat', i));
   [~, s] = size(DB);
   for j = 1:s
       C{counter, 1} = DB(j).z;
       C{counter, 2} = ifrdescp(C{counter,1},200);
       C{counter, 3} = bound2im(C{counter,2});
       s = regionprops(C{counter, 3}, 'ConvexHull','Perimeter');
       C{counter, 4} = s.ConvexHull;
       [sc, ~] = size(s.ConvexHull);
       difCH = zeros(size(s.ConvexHull));
       difCH(1,:) = s.ConvexHull(2,:) - s.ConvexHull(1,:);
       for k = 2:sc
           difCH(k,:) = s.ConvexHull(k - 1,:) - s.ConvexHull(k,:);
       end
       difCH = difCH.^2;
       d = difCH(:,1)+difCH(:,2);
       d = d.^(1/2);
       C{counter, 5} = sum(d);
       C{counter, 6} = s.Perimeter;
       C{counter, 7} = C{counter, 5}/s.Perimeter;
       ratio(counter) = C{counter, 7};
       counter = counter + 1;
   end
end

C(193,:) = [];

for i = 1:445
    
end
