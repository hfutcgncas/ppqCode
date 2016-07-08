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
    system(cmd); %������˷������
    pause(4); %��ʱ�Ա�֤��������ɶ������������ݡ�
    PMACData; % ��ȡ�������˶��켣���������OriData�С����ļ���PMAC����λ��������ɡ�
    cData = pmacDataProcess(OriData); %PMAC���ݸ�ʽת����
    save([savePath,num2str(i)],'cData','hitPar') %�洢��¼�����ݡ�   
end

