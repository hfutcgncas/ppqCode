HitPars = [0.3  0.5  0.27  2.2  1.1  2.2  0.2;
                -0.3  0.5  0.4  2.2  1.1  2.2  0.2;
                0.3  0.5  0.3  2.2  1.1  2.2  0.2];
%% 
addpath('temporaryData');
savePath = './SavedData/';
NumSample = size(HitPars,1);
for i = 1:NumSample
    cmd = '.\UDPSender\Debug\UDPSender.exe';
    hitPar = HitPars(i,:);
    for j = 1:7
        cmd = [cmd ,' ', num2str(hitPar(1,j))];
    end
    system(cmd); %向机器人发送命令。
    pause(4); %延时以保证机器人完成动作并传输数据。
    PMACData; % 读取机器人运动轨迹，结果放在OriData中。该文件由PMAC的上位机软件生成。
    cData = pmacDataProcess(OriData); %PMAC数据格式转换。
    save([savePath,num2str(i)],'cData','hitPar') %存储记录的数据。   
end

