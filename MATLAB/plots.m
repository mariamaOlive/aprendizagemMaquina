data= csvread('../tic-tac-toe-numerical.csv');    % Reading original file
%o,x,x,o,x,x,x,o,o,positive
a1=[0 1 1 0 1 1 1 0 0];
%o,o,x,o,x,x,b,b,x,positive
a2=[0 0 1 0 1 1 -1 -1 0];
%x,o,o,x,x,x,o,x,o,positive
b1=[1 0 0 1 1 1 0 1 0];
%x,b,o,x,o,x,o,x,o,negative
b2=[1 -1 0 1 0 1 0 1 0];


medoids=[a1;a2;b1;b2];
l=[1; 1; 2; 2];

atributes=data(1:626,1:9);
label=data(1:626, 10:10);
parallelcoords(medoids,'Group', l);





