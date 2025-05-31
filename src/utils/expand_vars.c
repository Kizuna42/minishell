/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:32:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/31 21:07:56 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_variable_value(char *var_name, t_minishell *shell)
{
	if (ft_strncmp(var_name, "?", 1) == 0 && ft_strlen(var_name) == 1)
		return (ft_itoa(shell->last_exit_status));
	else if (ft_strncmp(var_name, "$", 1) == 0 && ft_strlen(var_name) == 1)
		return (ft_itoa(getpid()));
	else if (ft_strlen(var_name) == 0)
		return (ft_strdup(""));
	else
		return (get_env_value(var_name, shell));
}

int	should_free_var_value(char *var_name)
{
	return ((ft_strncmp(var_name, "?", 1) == 0 && ft_strlen(var_name) == 1)
		|| (ft_strncmp(var_name, "$", 1) == 0 && ft_strlen(var_name) == 1)
		|| ft_strlen(var_name) == 0);
}

int	should_skip_variable(char *result, int i)
{
	return (result[i] == '\x01');
}
