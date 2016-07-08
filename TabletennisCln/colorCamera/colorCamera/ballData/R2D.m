close all
load X0.txt
M = X0;

M1 = M (M(:,3)==1,:);
M2 = M (M(:,3)==2,:);

plot(M1(:,1),-M1(:,2),'b.');
axis([0,656,-492,0]);
figure,
plot(M2(:,1),-M2(:,2),'b.');
axis([0,656,-492,0]);
 