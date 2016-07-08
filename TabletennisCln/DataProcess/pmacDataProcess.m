function cData = pmacDataProcess(OriData)
datalen = size(OriData,2);
NumGather = 6;
timeStep = floor(datalen/NumGather);
OriData = OriData(1:timeStep*NumGather);
Data = reshape(OriData,NumGather,timeStep);

cData =zeros(size(Data));

% �ŷ�������I7002���ã�������Ϊ3 �ŷ�Ƶ��Ϊ2.45kHz��
% ͬʱ����������Ϊ10���ŷ����ڣ����Բ������Ϊ 1/2450 * 10
time = 0:4.07918334007263E-03 :(4.07918334007263E-03 *(timeStep-1));
for i = 1:timeStep
    for j = 1:NumGather
        cData(j,i) = 0.000326*decodingPMAC(Data{j,i},'int');
    end
end

scaling = [187.5,187.5,136.05,136.05,187.5,187.5]';
bias = [0,0,0,0,320,320]';
cData = cData./repmat(scaling,1,timeStep);
cData = cData + repmat(bias,1,timeStep);

cData = [cData;time];
end
% 
% plot3(cData(2,:),cData(4,:),cData(6,:),'r')
% hold on
% plot3(cData(1,:),cData(3,:),cData(5,:),'b')
% hold off

