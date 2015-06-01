function [ ] = Lucas_Kanade( imgFirst, imgAfter )
%LUCAS_KANADE Summary of this function goes here

%Compute Ex, Ey, Et first
Ex = conv2(imgFirst, 0.25*[-1 1; -1 1], 'same') + conv2(imgAfter, 0.25*[-1 1; -1 1], 'same');
Ey = conv2(imgFirst, 0.25*[-1 -1; 1 1], 'same') + conv2(imgAfter, 0.25*[-1 -1; 1 1], 'same');
Et = conv2(imgFirst, 0.25*ones(2), 'same') + conv2(imgAfter, -0.25*ones(2), 'same');

%Initialize u&v
u = 0;
v = 0;

alpha = 0.5;

for I=1:100
    kernel = [1/12 1/6 1/12; 1/6 0 1/6; 1/12 1/6 1/12];
    
    uAvg = conv2(u, kernel, 'same');
    vAvg = conv2(v, kernel, 'same');
    
    u = uAvg-(Ex .* ((Ex .* uAvg)+(Ey.*vAvg)+Et)) ./ (alpha^2 + Ex.^2+Ey.^2);
    v = vAvg-(Ey .* ((Ex .* uAvg)+(Ey.*vAvg)+Et)) ./ (alpha^2 + Ex.^2+Ey.^2);
end


imshow(imgFirst)
[x,y] = meshgrid(1:2:386,1:2:386);
hold on
scale=5;
quiver(x,y,u(1:2:386,1:2:386),v(1:2:386,1:2:386),scale, 'color', 'b', 'linewidth', 0.001);


end

