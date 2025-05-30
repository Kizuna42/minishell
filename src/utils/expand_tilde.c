/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:46:34 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/30 17:49:35 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_tilde(char *str, t_minishell *shell)
{
	char	*home;
	char	*result;
	char	*temp;

	if (!str || str[0] != '~')
		return (ft_strdup(str));
	if (str[1] != '\0' && str[1] != '/')
		return (ft_strdup(str));
	home = get_env_value("HOME", shell);
	if (!home)
		return (ft_strdup(str));
	if (str[1] == '\0')
		return (ft_strdup(home));
	temp = ft_strjoin(home, str + 1);
	result = ft_strdup(temp);
	free(temp);
	return (result);
}

char	**expand_tilde_array(char **args, t_minishell *shell)
{
	char	**expanded_args;
	int		i;

	i = 0;
	while (args[i])
		i++;
	expanded_args = malloc(sizeof(char *) * (i + 1));
	if (!expanded_args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		expanded_args[i] = expand_tilde(args[i], shell);
		i++;
	}
	expanded_args[i] = NULL;
	return (expanded_args);
}
