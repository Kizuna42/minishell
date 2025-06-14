/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 04:10:19 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	parse_export_arg(char *arg, char **key, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		*key = ft_substr(arg, 0, equal_pos - arg);
		*value = ft_strdup(equal_pos + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}

static int	handle_invalid_option(char *arg, char *key, char *value)
{
	ft_putstr_fd("minishell: export: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": invalid option\n", STDERR_FILENO);
	ft_putstr_fd("export: usage: export [-nf] [name[=value] ...]",
		STDERR_FILENO);
	ft_putstr_fd(" or export -p\n", STDERR_FILENO);
	free(key);
	if (value)
		free(value);
	return (2);
}

static int	handle_invalid_identifier(char *arg, char *key, char *value)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	free(key);
	if (value)
		free(value);
	return (1);
}

static int	check_invalid_option(char *key, char *arg, char *value)
{
	if (key && key[0] == '-' && key[1] == '-')
		return (handle_invalid_option(arg, key, value));
	if (key && key[0] == '-' && key[1] && key[1] != '=')
		return (handle_invalid_option(arg, key, value));
	return (0);
}

int	process_export_arg(char *arg, t_minishell *shell)
{
	char	*key;
	char	*value;
	int		option_result;

	parse_export_arg(arg, &key, &value);
	option_result = check_invalid_option(key, arg, value);
	if (option_result)
		return (option_result);
	if (!is_valid_identifier(key))
		return (handle_invalid_identifier(arg, key, value));
	set_env_value(key, value, shell);
	free(key);
	if (value)
		free(value);
	return (0);
}
