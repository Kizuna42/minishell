/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:38:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/11 18:38:21 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_unset_option_error(char *arg)
{
	ft_putstr_fd("minishell: unset: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": invalid option\n", STDERR_FILENO);
	ft_putstr_fd("unset: usage: unset [-fv] [name ...]\n", STDERR_FILENO);
	return (2);
}

static int	handle_unset_identifier_error(char *arg)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (1);
}

int	process_unset_arg(char *arg, t_minishell *shell)
{
	if (arg[0] == '-' && arg[1])
		return (handle_unset_option_error(arg));
	else if (!is_valid_identifier(arg))
		return (handle_unset_identifier_error(arg));
	else
	{
		unset_env_value(arg, shell);
		return (0);
	}
} 
