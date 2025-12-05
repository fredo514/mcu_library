struct pid_ctx {
    pid_data_t last_output;
    pid_data_t setpoint;
    
    pid_data_t p_gain;
    pid_data_t i_gain;
    pid_data_t d_gain; 
    pid_data_t alpha_deriv;
    pid_data_t kaw; 
    float p_on_m_weight;

    pid_action_t action;
    pid_mode_t mode;

    // for integral
    pid_data_t i_term;
    pid_data_t last_error;

    // for derivative
    pid_data_t last_input;
    pid_data_t last_d_term;

    // for windup Clamping
    pid_data_t max_output;
    pid_data_t min_output;

    pid_error_calc_cb_t error_calc_cb;
};