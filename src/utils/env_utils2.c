/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 04:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 04:01:11 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_env_value(char *key, char *value, t_minishell *shell)
{
	t_env	*current;
	t_env	*new_env;

	current = shell->env_list;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
		{
			free(current->value);
			current->value = ft_strdup(value);
			return (0);
		}
		current = current->next;
	}
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (1);
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	new_env->next = shell->env_list;
	shell->env_list = new_env;
	return (0);
}

int	unset_env_value(char *key, t_minishell *shell)
{
	t_env	*current;
	t_env	*prev;

	current = shell->env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
		{
			if (prev)
				prev->next = current->next;
			else
				shell->env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}
