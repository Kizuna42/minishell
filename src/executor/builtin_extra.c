/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_extra.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/31 20:28:33 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <limits.h>

int	builtin_unset(char **args, t_minishell *shell)
{
	int	i;
	int	exit_status;

	if (!args[1])
		return (0);
	i = 1;
	exit_status = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = 1;
		}
		else
		{
			unset_env_value(args[i], shell);
		}
		i++;
	}
	return (exit_status);
}

static int	is_numeric_string(char *str)
{
	int		i;
	char	*endptr;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	errno = 0;
	strtol(str, &endptr, 10);
	if (*endptr != '\0')
		return (0);
	if (errno == ERANGE)
		return (0);
	return (1);
}

static void	print_exit_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

int	builtin_exit(char **args, t_minishell *shell)
{
	int		exit_code;
	long	num;

	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!args[1])
	{
		cleanup_minishell(shell);
		exit(shell->last_exit_status);
	}
	if (!is_numeric_string(args[1]))
	{
		print_exit_error(args[1]);
		cleanup_minishell(shell);
		exit(255);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	num = ft_atoi(args[1]);
	exit_code = (unsigned char)num;
	cleanup_minishell(shell);
	exit(exit_code);
}

int	builtin_export(char **args, t_minishell *shell)
{
	int	i;
	int	exit_status;

	if (!args[1])
		return (print_export_env(shell));
	i = 1;
	exit_status = 0;
	while (args[i])
	{
		if (process_export_arg(args[i], shell))
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
