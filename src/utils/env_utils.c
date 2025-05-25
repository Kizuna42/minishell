/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 20:52:47 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_env	*add_oldpwd_env(t_env *env_list)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));
	if (new_env)
	{
		new_env->key = ft_strdup("OLDPWD");
		new_env->value = NULL;
		new_env->next = env_list;
		env_list = new_env;
	}
	return (env_list);
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*new_env;
	char	*equal_pos;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		equal_pos = ft_strchr(envp[i], '=');
		if (equal_pos)
		{
			new_env = malloc(sizeof(t_env));
			if (!new_env)
				return (NULL);
			new_env->key = ft_substr(envp[i], 0, equal_pos - envp[i]);
			new_env->value = ft_strdup(equal_pos + 1);
			new_env->next = env_list;
			env_list = new_env;
		}
		i++;
	}
	return (add_oldpwd_env(env_list));
}

char	*get_env_value(char *key, t_minishell *shell)
{
	t_env	*current;

	if (ft_strncmp(key, "?", 1) == 0)
		return (ft_itoa(shell->last_exit_status));
	current = shell->env_list;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

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
