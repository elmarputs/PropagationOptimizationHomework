subplot(1, 2, 1);
plot(t, cowell(:, 2), t, kepler(:, 2));
legend('RK 4 Cowell', 'RK 4 Kepler', 'Location', 'northwest');
grid('on');
title('\Delta x');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Position error [m]');

subplot(1, 2, 2);
plot(t, cowell(:, 5), t, kepler(:, 5));
legend('RK 4 Cowell', 'RK 4 Kepler', 'Location', 'northwest');
grid('on');
title('\Delta V_x');
xlim([0 3.5]);
xlabel('Time [days]');
ylabel('Velocity error [m/s]');