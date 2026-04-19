distance   = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100];
resistance = [58.2, 107.2, 144.3, 189, 253, 305, 373, 446, 511, 561];

% 線形近似
p = polyfit(distance, resistance, 1);
fit_x = linspace(0, 110, 100);
fit_y = polyval(p, fit_x);

% 決定係数 R^2
resistance_fit = polyval(p, distance);
ss_res = sum((resistance - resistance_fit).^2);
ss_tot = sum((resistance - mean(resistance)).^2);
R2 = 1 - ss_res / ss_tot;

figure;
hold on;

plot(distance, resistance, 'o', 'MarkerSize', 8, 'DisplayName', '測定値');
plot(fit_x, fit_y, '--', 'DisplayName', sprintf('近似直線: y = %.4fx + %.4f\nR^2 = %.4f', p(1), p(2), R2));

xlabel('2点間の距離 [mm]');
ylabel('抵抗値 [kΩ]');
legend('Location', 'northwest');
grid on;
xlim([0 110]);
ylim([0 600]);

hold off;
