/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/31 19:59:57 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(char *command)
{
	if (!command)
		return (0);
	if (ft_strncmp(command, "echo", 4) == 0 && ft_strlen(command) == 4)
		return (1);
	if (ft_strncmp(command, "cd", 2) == 0 && ft_strlen(command) == 2)
		return (1);
	if (ft_strncmp(command, "pwd", 3) == 0 && ft_strlen(command) == 3)
		return (1);
	if (ft_strncmp(command, "export", 6) == 0 && ft_strlen(command) == 6)
		return (1);
	if (ft_strncmp(command, "unset", 5) == 0 && ft_strlen(command) == 5)
		return (1);
	if (ft_strncmp(command, "env", 3) == 0 && ft_strlen(command) == 3)
		return (1);
	if (ft_strncmp(command, "exit", 4) == 0 && ft_strlen(command) == 4)
		return (1);
	return (0);
}

int	execute_builtin(char **args, t_minishell *shell)
{
	if (!args || !args[0])
		return (1);
	if (ft_strncmp(args[0], "echo", 4) == 0 && ft_strlen(args[0]) == 4)
		return (builtin_echo(args));
	else if (ft_strncmp(args[0], "cd", 2) == 0 && ft_strlen(args[0]) == 2)
		return (builtin_cd(args, shell));
	else if (ft_strncmp(args[0], "pwd", 3) == 0 && ft_strlen(args[0]) == 3)
		return (builtin_pwd());
	else if (ft_strncmp(args[0], "export", 6) == 0 && ft_strlen(args[0]) == 6)
		return (builtin_export(args, shell));
	else if (ft_strncmp(args[0], "unset", 5) == 0 && ft_strlen(args[0]) == 5)
		return (builtin_unset(args, shell));
	else if (ft_strncmp(args[0], "env", 3) == 0 && ft_strlen(args[0]) == 3)
		return (builtin_env(args, shell));
	else if (ft_strncmp(args[0], "exit", 4) == 0 && ft_strlen(args[0]) == 4)
		return (builtin_exit(args, shell));
	return (1);
}
