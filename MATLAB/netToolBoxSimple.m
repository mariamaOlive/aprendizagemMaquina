% Solve a Pattern Recognition Problem with a Neural Network
% Script generated by Neural Pattern Recognition app
% Created 19-Nov-2015 18:31:13
%
% This script assumes these variables are defined:
%
%   cancerInputs - input data.
%   cancerTargets - target data.

fileID = fopen('errorMLP.txt','w');

for j=0:9 %Reapeat k-fold 10x
    
    stVal=1;
    stTest=96;
    foldSize=95;
    stTrng2=stTest+foldSize;
    nSample=950;
    s=sprintf('../data_kfold/MLP/MLP%i.csv', j);
    data= csvread(s)';    % Reading file of training, validation and test

    for i=0:9

    %k-fold 10 cases
    if i==0
            [trainInd,valInd,testInd] = divideind(nSample,stTrng2:nSample,stVal:(stVal+foldSize-1),stTest:(stTest+foldSize-1));
            stVal=stVal+foldSize;
            stTest=stTest+foldSize;
            stTrng2=stTest+foldSize;
            stTrng1=1;
    elseif (i==1 || i==2 ||i==3 ||i==4 ||i==5 ||i==6 ||i==7)
            [trainInd,valInd,testInd] = divideind(nSample,[stTrng1:(stVal-1),stTrng2:nSample],stVal:(stVal+foldSize-1),stTest:(stTest+foldSize-1));
            stVal=stVal+foldSize;
            stTest=stTest+foldSize;
            stTrng2=stTest+foldSize;
            stTrng1=1;
    else
          [trainInd,valInd,testInd] = divideind(nSample,stTrng1:(stVal-1),stVal:(stVal+foldSize-1),stTest:(stTest+foldSize-1));
          stVal=stVal+foldSize;
          stTest=1;
          stTrng1=foldSize+1;     
    end

    nInput=950;
    nAtributes=9;
    
    x = data(1:nAtributes, 1:nInput);
    t = data((nAtributes + 1):11, 1:nInput);


    % Choose a Training Function
    % For a list of all training functions type: help nntrain
    % 'trainlm' is usually fastest.
    % 'trainbr' takes longer but may be better for challenging problems.
    % 'trainscg' uses less memory. Suitable in low memory situations.
    trainFcn = 'trainscg';  % Scaled conjugate gradient backpropagation.

    % Create a Pattern Recognition Network
    hiddenLayerSize = 6;
    net = patternnet(hiddenLayerSize);

    % Setup Division of Data for Training, Validation, Testing
    net.divideFcn = 'divideind';
    net.divideMode = 'sample';  % Divide up every sample

    net.divideParam.trainInd = trainInd;
    net.divideParam.valInd = valInd;
    net.divideParam.testInd = testInd;
    % Train the Network
    [net,tr] = train(net,x,t);

    % Test the Network
    tInput=x(:,tr.testInd);
    tLabel=t(:,tr.testInd);

    y = net(tInput);
    %e = gsubtract(t,y);
    %performance = perform(net,t,y)
    tind = vec2ind(tLabel);
    yind = vec2ind(y);
    percentErrors = sum(tind ~= yind)/numel(tind);
    fprintf(fileID,'%f\n',percentErrors);

    disp(percentErrors);

    % View the Network
    %view(net)

    % Plots
    % Uncomment these lines to enable various plots.
    %figure, plotperform(tr)
    %figure, plottrainstate(tr)
    %figure, ploterrhist(e)
    %figure, plotconfusion(tLabel,y)
    %figure, plotroc(t(tr.testInd),y)

    startValidation=startValidation+95;
    startTest=startTest+95;
    end
end

fclose(fileID);