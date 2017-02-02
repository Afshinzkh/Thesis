function getAllPlot(vasicek,vasicekGPU, cir, cirGPU, j)
        tau = [0.25, 1, 3, 5, 7, 10, 15, 20, 30]';
        market = csvread('data.csv');
        hold on
             
        plot(tau,market(13-j,:),'-k','DisplayName','Market');
        plot(tau,vasicek(j,:),'--ro','DisplayName','Vasicek');
        plot(tau,cir(j,:),'--bo','DisplayName','CIR');
        plot(tau,vasicekGPU(j,:),'-.g*','DisplayName','VasicekGPU');
        plot(tau,cirGPU(j,:),'-.c*','DisplayName','CIR-GPU');
        
        legend('show')
end