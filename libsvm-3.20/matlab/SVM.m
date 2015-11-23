fileID = fopen('errorSVM.txt', 'w');

for j=0:9
    for i=0 :9
        nome=sprintf('../../data_kfold/SVM/%i/SVMtraining%i.csv',j, i);
        dataTraining= csvread(nome);

        [nInput,nAtributes]=size(dataTraining);
        nAtributes=nAtributes -1;
        training_instance_matrix = dataTraining(1:nInput, 1:nAtributes);
        training_label_vector= dataTraining(1:nInput, (nAtributes+1):10);

        model = svmtrain(training_label_vector, training_instance_matrix, '');

        nometest=sprintf('../../data_kfold/SVM/%i/SVMtest%i.csv',j, i);
        dataTest= csvread(nometest);

        [nInput,nAtributes]=size(dataTest);
        nAtributes=nAtributes -1;
        test_instance_matrix = dataTest(1:nInput, 1:nAtributes);
        test_label_vector= dataTest(1:nInput, (nAtributes+1):10);

       [predicted_label, accuracy, decision] = svmpredict(test_label_vector, test_instance_matrix, model, '');
 
       percentErrors = sum(test_label_vector ~=predicted_label)/numel(test_label_vector);
       
       fprintf(fileID, '%f\n', percentErrors);
       
         i=0;
    end;
end;

fclose(fileID);


