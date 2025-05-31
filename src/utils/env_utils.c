/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 04:01:11 by kizuna           ###   ########.fr       */
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

static t_env	*create_env_node(char *envp_line)
{
	t_env	*new_env;
	char	*equal_pos;

	equal_pos = ft_strchr(envp_line, '=');
	if (!equal_pos)
		return (NULL);
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	new_env->key = ft_substr(envp_line, 0, equal_pos - envp_line);
	new_env->value = ft_strdup(equal_pos + 1);
	new_env->next = NULL;
	return (new_env);
}

static void	add_env_node(t_env **env_list, t_env **current, t_env *new_env)
{
	if (!*env_list)
		*env_list = new_env;
	else
		(*current)->next = new_env;
	*current = new_env;
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*new_env;
	t_env	*current;
	int		i;

	env_list = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		new_env = create_env_node(envp[i]);
		if (new_env)
			add_env_node(&env_list, &current, new_env);
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
