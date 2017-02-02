function getPlot(results,i,j)
   if nargin == 2
        tau = [0.25, 1, 3, 5, 7, 10, 15, 20, 30]';
        market = csvread('data.csv');
        model = results;
        hold on
        col = rand(1,3);
        plot(tau,market(13-i,:),'color', col);
        plot(tau,model(i,:),'--', 'color' , col);

   end
   
   if nargin == 1
        tau = [0.25, 1, 3, 5, 7, 10, 15, 20, 30]';
        market = csvread('data.csv');
        model = results;
        hold on
    
        for i=1:12
            col = rand(1,3);
            plot(tau,market(13-i,:),'color', col);
            plot(tau,model(i,:),'--', 'color' , col);
        end
   end
   
   if nargin == 3
        tau = [0.25, 1, 3, 5, 7, 10, 15, 20, 30]';
        market = csvread('data.csv');
        model = results;
        hold on
    
        for i=i:j
            col = rand(1,3);
            plot(tau,market(13-i,:),'color', col);
            plot(tau,model(i,:),'--', 'color' , col);
        end
   end
end