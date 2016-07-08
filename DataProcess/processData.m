close all

%%
%addpath('E:\\liu\\ppqCodeV2\\colorCameraV5 - 副本\\colorCamera\\temporaryData')
addpath('..\\Debug\\temporaryData')
addpath('E:\\liu\\cal')

load 3Dtrace.txt
load HitPoint.txt

load 192.168.64.108.txt
load 192.168.64.109.txt

x0 = X192_168_64_108;
x1 = X192_168_64_109;
x2= X3Dtrace;
x3 = HitPoint;
%
subplot(2,2,1)
im100 = imread('Cam100Ex_3_21.bmp');
imshow(im100);
hold on
plot(x0(:,1),x0(:,2),'r.');
title('108')
%
subplot(2,2,2)
im101 = imread('Cam101Ex_3_21.bmp');
imshow(im101);
hold on
plot(x1(:,1),x1(:,2),'b.');
title('109')
%
subplot(2,2,3)
plot3(x2(:,1),x2(:,2),x2(:,3),'b.');
axis([-1.3,1.3,-0.8,0.8,0,1])
xlabel('X')
ylabel('Y')
zlabel('Z')
axis auto equal
grid
title('ppq 3D')

hold on
plot3(x3(:,1),x3(:,2),x3(:,3),'rx');
axis([-1.3,1.3,-0.8,0.8,0,2])
%%
PMACData
cData = pmacDataProcess(OriData); %PMAC数据格式转换。

subplot(2,2,4)
plot3(cData(1,:),cData(3,:),cData(5,:),'g.')
axis([-1.3,1.3,-0.8,0.8,0,1])
xlabel('X')
ylabel('Y')
zlabel('Z')
axis auto equal
grid
title('bat 3D')