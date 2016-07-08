close all

addpath('..\\Debug\\temporaryData')
addpath('E:\\liu\\cal')

%% 读取ini文件
addpath('INI')
File = 'E:\liu\ppqCodeV3\TabletennisCln\Debug\historyData\cfg.ini';
I = INI('File',File);
I.read();
Sections = I.get('Sections');
fileIndex = I.get(Sections{1}).FileIndex;
DirName = I.get(Sections{1}).DirName;

TraceFile =  ['..\\Debug\\',DirName,'\\',int2str(fileIndex),'_3Dtrace.txt'];
HitPointFile =  ['..\\Debug\\',DirName,'\\',int2str(fileIndex),'_HitPoint.txt'];
imgPointAFile =  ['..\\Debug\\',DirName,'\\',int2str(fileIndex),'_imgPointA.txt'];
imgPointBFile =  ['..\\Debug\\',DirName,'\\',int2str(fileIndex),'_imgPointB.txt'];
PMACDataFile = ['..\\Debug\\',DirName,'\\A',int2str(fileIndex),'_PMACData.m'];

x0 = load(imgPointAFile);
x1 = load(imgPointBFile);
X3Dtrace = load(TraceFile);
HitPoint = load(HitPointFile);
%%

% %%
% load 3Dtrace.txt
% load HitPoint.txt
% 
% load 192.168.64.108.txt
% load 192.168.64.109.txt
% 
% x0 = X192_168_64_108;
% x1 = X192_168_64_109;
% x2= X3Dtrace;
% x3 = HitPoint;
%
%%  画图
subplot(1,2,1)
im100 = imread('Cam108Blank.bmp');
imshow(im100);
hold on
plot(x0(:,1),x0(:,2),'r.');
title('108')
%
subplot(1,2,2)
im101 = imread('Cam109Blank.bmp');
imshow(im101);
hold on
plot(x1(:,1),x1(:,2),'b.');
title('109')
%%  画击球轨迹

figure,
plot3(X3Dtrace(:,1),X3Dtrace(:,2),X3Dtrace(:,3),'b.');
axis([-1.3,1.3,-0.8,0.8,0,1])
xlabel('X')
ylabel('Y')
zlabel('Z')
axis auto equal
grid
title('ppq 3D')

hold on
plot3(HitPoint(:,1),HitPoint(:,2),HitPoint(:,3),'rx');
axis([-1.3,1.3,-0.8,0.8,0,2])
%%
% PMACData
run([PMACDataFile])
cData = pmacDataProcess(OriData); %PMAC数据格式转换。
cData(7,:)=cData(7,:)+HitPoint(1,8)/1000; 

cDataX = 1.37-cData(4,:)/1000 ;
cDataY = cData(2,:)/1000;
cDataZ = cData(6,:)/1000;

hold on
plot3(cDataX(:),cDataY(:),cDataZ(:),'g.')
xlabel('X')
ylabel('Y')
zlabel('Z')
axis auto equal
grid
title('bat 3D')
% 
% subplot(2,2,4)
% plot3(cDataX(:),cDataY(:),cDataZ(:),'g.')
% axis([-1.3,1.3,-0.8,0.8,0,1])

%%
figure,
subplot(1,3,1)
 plot(cData(7,:),cData(4,:),'r.')
 hold on
 plot(X3Dtrace(:,4),1370-X3Dtrace(:,1)*1000,'b.')
 plot(HitPoint(:,4),1370-HitPoint(:,1)*1000,'g.')
 title('Y')
 
 subplot(1,3,2)
 plot(cData(7,:),cData(2,:),'r.')
 hold on
 plot(X3Dtrace(:,4),X3Dtrace(:,2)*1000,'b.')
 plot(HitPoint(:,4),HitPoint(:,2)*1000,'g.')
  title('X')
 
 subplot(1,3,3)
 plot(cData(7,:),cData(6,:),'r.')
 hold on
 plot(X3Dtrace(:,4),X3Dtrace(:,3)*1000,'b.')
 plot(HitPoint(:,4),HitPoint(:,3)*1000,'g.')
 title('Z')
 
 %%
 figure,
 plot( cData(7,1:(end-1)),(cData(3,2:end)-cData(3,1:end-1) )./(cData(7,2:end)-cData(7,1:(end-1))),'r.')
 hold on
 plot(HitPoint(:,4),HitPoint(:,3)*1000,'g.')
 grid
 