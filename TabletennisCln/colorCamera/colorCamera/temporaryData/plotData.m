close all

load 3Dtrace.txt
load HitPoint.txt
load 192.168.64.108.txt
load 192.168.64.109.txt

x0 = X192_168_64_108;
x1 = X192_168_64_109;
x2= X3Dtrace;
x3 = HitPoint;

figure,
im100 = imread('.\\Cam100Ex_3_21.bmp');
imshow(im100);
hold on
plot(x0(:,1),x0(:,2),'r.');
title('108')

figure,
im101 = imread('.\\Cam101Ex_3_21.bmp');
imshow(im101);
hold on
plot(x1(:,1),x1(:,2),'b.');
title('109')

figure,
plot3(x2(:,1),x2(:,2),x2(:,3),'b.');
axis([-1.3,1.3,-0.8,0.8,0,2])
title('3D')

hold on
plot3(x3(:,1),x3(:,2),x3(:,3),'rx');
axis([-1.3,1.3,-0.8,0.8,0,2])




