/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:15:14 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**expand_variables_array(char **args, t_minishell *shell)
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
		expanded_args[i] = expand_variables(args[i], shell);
		i++;
	}
	expanded_args[i] = NULL;
	return (expanded_args);
}

char	*remove_quote_markers(char *str)
{
	char	*result;
	int		i;

	result = ft_strdup(str);
	i = 0;
	while (result[i])
	{
		if (result[i] == '\x01' || result[i] == '\x02')
		{
			memmove(result + i, result + i + 1, ft_strlen(result + i));
			continue ;
		}
		i++;
	}
	return (result);
}
