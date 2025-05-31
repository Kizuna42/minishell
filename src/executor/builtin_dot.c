/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dot.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:41:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/31 20:40:58 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_dot(char **args, t_minishell *shell)
{
	(void)shell;
	if (!args[1])
	{
		ft_putstr_fd(".: filename argument required\n", STDERR_FILENO);
		ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO);
		return (2);
	}
	ft_putstr_fd(".: ", STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	return (2);
}
