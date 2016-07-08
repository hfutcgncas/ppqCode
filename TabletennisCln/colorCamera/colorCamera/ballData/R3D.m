close all

%% get filePath
path = '球轨迹数据_';
path = [path,num2str(year(date)),'_' ];
if month(date) < 10
    path = [path,'0' ];
end
path = [path,num2str(month(date)),'_' ];
if day(date) < 10
    path = [path,'0' ];
end
path = [path,num2str(day(date)) ];
files = dir(fullfile(path,'*.txt'));
if size(files,1)==0
    disp('没有文件');
end

iniFile = importdata('sys.ini');
fileIndex = str2num(iniFile{2}(11:end));
file = [path,'\','ballData',num2str(fileIndex-1),'.txt'];

M = importdata(file);

plot3(M(:,1)-1.1,M(:,2)-0.3,M(:,3),'b.');
xlabel('X');ylabel('Y');zlabel('Z');
axis([-2.37,2.37,-1.06,1.06,0,2]);
hold off
 